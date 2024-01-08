#include "GravityGun/GravityGunComponent.h"
#include "Character/CustomCharacter.h"
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
}

void UGravityGunComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGravityGunComponent::SetupCharacter(ACustomCharacter* character_)
{
	character = character_;
}


void UGravityGunComponent::TakeObject()
{
	if (!cameraManager.IsValid()) return;
	if (!IsValid(character)) return;


	FVector start = cameraManager->GetCameraLocation();
	FVector end = start + cameraManager->GetActorForwardVector() * GravityGunRange;

	FCollisionQueryParams params;
	params.AddIgnoredActor(character);

	FHitResult hit;

	bool result = GetWorld()->LineTraceSingleByChannel(hit, start, end, gravityGunCollisionChannel, params);

	kPRINT(result ? "Gravity Guned an object " + hit.GetActor()->GetActorNameOrLabel() : "Gravity Gun found nothing");
}

void UGravityGunComponent::ThrowObject()
{
	kPRINT("Gravity Gun : Throw");


}

