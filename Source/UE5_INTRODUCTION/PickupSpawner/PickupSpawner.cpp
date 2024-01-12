#include "PickupSpawner/PickupSpawner.h"
#include "Character/CustomCharacter.h"
#include "GravityGun/GravityGunComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Defines.h"

UPickupSpawner::UPickupSpawner()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPickupSpawner::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APickup::StaticClass(), actors);

	for (auto actor : actors)
	{
		APickup* pickup = Cast<APickup>(actor);
		if (!IsValid(pickup)) continue;

		if (!numberPickups.Contains(pickup->GetPickupType())) continue;

		numberPickups[pickup->GetPickupType()]++;
		pickup->OnPickupDestroyWithType.AddUniqueDynamic(this, &UPickupSpawner::OnPickupDestroyed);
	}
}

void UPickupSpawner::SetupCharacter(ACustomCharacter* character_)
{
	character = character_;

	gravityGun = character->GetComponentByClass<UGravityGunComponent>();
}

void UPickupSpawner::SpawnPickup(EPickupType pickupType)
{
	if (!bSpawnable)
	{
		kPRINT_COLOR("Spawner in delay !", FColor::Yellow);
		return;
	}

	if (!numberPickups.Contains(pickupType) || !MaxNumberPickups.Contains(pickupType)) return;

	if (numberPickups[pickupType] + 1 > MaxNumberPickups[pickupType])
	{
		kPRINT_COLOR("There is too many pickups of type " + UEnum::GetValueAsString(pickupType) + " on the stage !", FColor::Red);
		return;
	}
	if (GetNumberAllTypesPickups() + 1 > MaxPickupAllTypes)
	{
		kPRINT_COLOR("There is too many pickups (all types) on the stage !", FColor::Red);
		return;
	}

	kPRINT("Spawn pickup of type " + UEnum::GetValueAsString(pickupType));

	TSubclassOf<APickup> pickup_spawn_class = APickup::StaticClass();
	switch (pickupType)
	{
	case EPickupType::Standard:
		pickup_spawn_class = PickupStandard;
		break;

	case EPickupType::DestroyAfterThrow:
		pickup_spawn_class = PickupDestroyAfterThrow;
		break;

	case EPickupType::DestroyAfterTake:
		pickup_spawn_class = PickupDestroyAfterTake;
		break;

	default:
		return;
	}

	APickup* pickup = Cast<APickup>(GetWorld()->SpawnActor(pickup_spawn_class));
	if (!IsValid(pickup)) return;

	if (IsValid(gravityGun) && !gravityGun->HasObjectInHand())
	{
		gravityGun->SetObjectInHand(pickup);
	}
	else
	{
		pickup->SetActorLocation(character->GetActorLocation() +
			character->GetActorForwardVector() * PickupSpawnOffsetFromPlayer.X +
			character->GetActorRightVector() * PickupSpawnOffsetFromPlayer.Y +
			character->GetActorUpVector() * PickupSpawnOffsetFromPlayer.Z);
		pickup->SetActorRotation(character->GetActorQuat());
	}

	pickup->OnPickupDestroyWithType.AddUniqueDynamic(this, &UPickupSpawner::OnPickupDestroyed);

	numberPickups[pickupType]++;

	if (DelayBetweenTwoSpawn <= 0.0f) return;
 	bSpawnable = false;
	ClearTimer();
	FTimerManager& timer_manager = character->GetWorldTimerManager();
	timer_manager.SetTimer(pickupSpawnDelayTimerHandle, this, &UPickupSpawner::SetSpawnableTrue, DelayBetweenTwoSpawn, false);
}

void UPickupSpawner::PrintPickupInfos()
{
	kPRINT("=========================================");
	kPRINT("Total number of pickups (all types) : " + FString::FromInt(GetNumberAllTypesPickups()));
	kPRINT("Number of pickups Destroy After Take : " + FString::FromInt(numberPickups[EPickupType::DestroyAfterTake]));
	kPRINT("Number of pickups Destroy After Throw : " + FString::FromInt(numberPickups[EPickupType::DestroyAfterThrow]));
	kPRINT("Number of pickups Standard : " + FString::FromInt(numberPickups[EPickupType::Standard]));
	kPRINT("=========================================");
}

void UPickupSpawner::OnPickupDestroyed(EPickupType pickupType)
{
	kPRINT("Pickup of type " + UEnum::GetValueAsString(pickupType) + " has been destroyed");

	if (!numberPickups.Contains(pickupType)) return;

	numberPickups[pickupType]--;
}


int UPickupSpawner::GetNumberAllTypesPickups()
{
	int total_number = 0;
	for (auto number_pickup : numberPickups)
	{
		total_number += number_pickup.Value;
	}
	return total_number;
}

void UPickupSpawner::SetSpawnableTrue()
{
	ClearTimer();

	bSpawnable = true;
}

void UPickupSpawner::ClearTimer()
{
	FTimerManager& timer_manager = character->GetWorldTimerManager();
	timer_manager.ClearTimer(pickupSpawnDelayTimerHandle);
}

