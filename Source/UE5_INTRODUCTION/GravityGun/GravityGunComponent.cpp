#include "GravityGun/GravityGunComponent.h"
#include "Character/CustomCharacter.h"
#include "Pickup/Pickup.h"
#include "Kismet/GameplayStatics.h"
#include "Defines.h"

UGravityGunComponent::UGravityGunComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGravityGunComponent::BeginPlay()
{
	Super::BeginPlay();

	cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	gravityGunCollisionChannel = UEngineTypes::ConvertToCollisionChannel(GravityGunTraceChannel);

	GravityGunRange = FMath::Clamp(GravityGunRange, CustomRange.Min, CustomRange.Max);

	pickupOffsetXAtMaxCharge = PickupOffsetFromPlayer.X * 0.7f;
}

void UGravityGunComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	kPRINT_TICK("Gravity Gun | Current range : " + FString::SanitizeFloat(GravityGunRange));

	if (bAccumulatingForce)
	{
		complexForceTimeAccumulated = FMath::Min(complexForceTimeAccumulated + DeltaTime, PickupComplexThrowForceMaxTime);
	}


	if (!IsValid(currentPickup)) return;

	FVector pickup_location = cameraManager->GetCameraLocation();
	pickup_location += cameraManager->GetActorForwardVector() * (bAccumulatingForce
			? FMath::Lerp(PickupOffsetFromPlayer.X, pickupOffsetXAtMaxCharge, complexForceTimeAccumulated / PickupComplexThrowForceMaxTime)
			: PickupOffsetFromPlayer.X);
	pickup_location += cameraManager->GetActorRightVector() * PickupOffsetFromPlayer.Y;
	pickup_location += cameraManager->GetActorUpVector() * PickupOffsetFromPlayer.Z;

	FQuat pickup_rotation = cameraManager->GetActorQuat();
	currentPickup->SetActorLocationAndRotation(pickup_location, pickup_rotation);
}

void UGravityGunComponent::SetupCharacter(ACustomCharacter* character_)
{
	character = character_;
}


void UGravityGunComponent::TakeObject()
{
	if (IsValid(currentPickup)) //  can't pickup a new object if gravity gun already have one
	{
		ReleaseObject();
		return;
	}

	if (!cameraManager.IsValid()) return;
	if (!IsValid(character)) return;


	FVector start = cameraManager->GetCameraLocation();
	FVector end = start + cameraManager->GetActorForwardVector() * GravityGunRange;

	FCollisionQueryParams params;
	params.AddIgnoredActor(character);

	FHitResult hit;

	bool result = GetWorld()->LineTraceSingleByChannel(hit, start, end, gravityGunCollisionChannel, params);

#if !UE_BUILD_SHIPPING
	if (bDrawDebugRaycast)
	{
		DrawDebugLine(GetWorld(), start, end, result ? FColor::Green : FColor::Red, false, TimeDebugRaycast, 0, 2.0f);
	}
#endif

	if (!result) return;

	APickup* pickup = Cast<APickup>(hit.GetActor());

	if (!IsValid(pickup)) return;

	SetObjectInHand(pickup);
}

void UGravityGunComponent::ReleaseObject()
{
	if (IsValid(pickupMesh))
	{
		if (currentPickup->GetPickupType() == EPickupType::DestroyAfterTake)
		{
			currentPickup->OnPickupDestroy.RemoveDynamic(this, &UGravityGunComponent::OnHoldPickupDestroyed);
		}

		pickupMesh->SetCollisionProfileName(pickupBaseCollisionProfile);
		pickupMesh->SetSimulatePhysics(true);

		pickupBaseCollisionProfile = FName();
		pickupMesh = nullptr;
	}

	currentPickup = nullptr;

	bAccumulatingForce = false;
	complexForceTimeAccumulated = 0.0f;
}

void UGravityGunComponent::ComplexThrowForceAccumulation()
{
	if (!IsValid(currentPickup)) return;

	bAccumulatingForce = true;
	complexForceTimeAccumulated = 0.0f;
}

void UGravityGunComponent::ThrowObject()
{
	if (!IsValid(currentPickup)) return;

	if (IsValid(pickupMesh))
	{
		pickupMesh->SetCollisionProfileName(pickupBaseCollisionProfile);
		pickupMesh->SetSimulatePhysics(true);

		//  complex throw direction
		FVector start = cameraManager->GetCameraLocation();
		FVector end = start + cameraManager->GetActorForwardVector() * 10000.0f;

		FCollisionQueryParams params;
		params.AddIgnoredActor(character);

		FHitResult hit;

		bool result = GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Visibility, params);

		FVector throw_aim = end;
		if (result)
		{
			throw_aim = hit.ImpactPoint;
		}

		FVector throw_direction = throw_aim - currentPickup->GetActorLocation();
		throw_direction.Normalize();


		if (bUseComplexThrowForce)
		{
			float complex_throw_force = PickupComplexThrowForce.Interpolate(complexForceTimeAccumulated / PickupComplexThrowForceMaxTime);
			pickupMesh->AddImpulse(throw_direction * complex_throw_force, NAME_None, true);
		}
		else
		{
			pickupMesh->AddImpulse(throw_direction * PickupThrowForce, NAME_None, true);
		}


		FVector angular_throw = PickupAngularThrow;
		if(bUseRandomAngularThrow) angular_throw = FVector(FMath::RandRange(-10000.0f, 10000.0f), FMath::RandRange(-10000.0f, 10000.0f), FMath::RandRange(-10000.0f, 10000.0f));
		if (bUseComplexThrowForce) angular_throw *= PickupComplexThrowForce.Interpolate(complexForceTimeAccumulated / PickupComplexThrowForceMaxTime) / PickupComplexThrowForce.Max;

		pickupMesh->AddAngularImpulseInDegrees(angular_throw, NAME_None, true);

		if(currentPickup->GetPickupType() == EPickupType::DestroyAfterThrow)
		{
			currentPickup->StartPickupDestructionTimer();
		}

		pickupBaseCollisionProfile = FName();
		pickupMesh = nullptr;
	}

	currentPickup = nullptr;

	bAccumulatingForce = false;
	complexForceTimeAccumulated = 0.0f;
}

void UGravityGunComponent::UpdateRange(float change)
{
	GravityGunRange = FMath::Clamp(GravityGunRange + change, CustomRange.Min, CustomRange.Max);
}

void UGravityGunComponent::DestroyPickupInHand()
{
	if (!IsValid(currentPickup)) return;

	currentPickup->Destroy();

	pickupBaseCollisionProfile = FName();
	pickupMesh = nullptr;
	currentPickup = nullptr;
}

void UGravityGunComponent::SetObjectInHand(APickup* pickup)
{
	if (!IsValid(pickup)) return;

	if (IsValid(currentPickup)) return; //  can't set a pickup in hand if gravity gun already have a pickup in hand

	currentPickup = pickup;

	pickupMesh = currentPickup->GetComponentByClass<UStaticMeshComponent>();

	if (!IsValid(pickupMesh)) return;

	pickupBaseCollisionProfile = pickupMesh->GetCollisionProfileName();
	pickupMesh->SetSimulatePhysics(false);
	pickupMesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	if (currentPickup->GetPickupType() == EPickupType::DestroyAfterThrow)
	{
		currentPickup->ClearTimer();
	}

	if (currentPickup->GetPickupType() == EPickupType::DestroyAfterTake)
	{
		currentPickup->StartPickupDestructionTimer();

		currentPickup->OnPickupDestroy.AddUniqueDynamic(this, &UGravityGunComponent::OnHoldPickupDestroyed);
	}

	OnPickupTaken.Broadcast(currentPickup->GetActorNameOrLabel());

	bAccumulatingForce = false;
	complexForceTimeAccumulated = 0.0f;
}

bool UGravityGunComponent::HasObjectInHand() 
{ 
	return IsValid(currentPickup); 
}

void UGravityGunComponent::OnHoldPickupDestroyed()
{
	if (!IsValid(currentPickup)) return;

	currentPickup->OnPickupDestroy.RemoveDynamic(this, &UGravityGunComponent::OnHoldPickupDestroyed);

	pickupBaseCollisionProfile = FName();
	pickupMesh = nullptr;
	currentPickup = nullptr;
}

