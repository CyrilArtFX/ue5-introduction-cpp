#include "PickupSpawner/PickupSpawnerController.h"
#include "Character/CustomCharacter.h"
#include "PickupSpawner.h"
#include "Defines.h"

UPickupSpawnerController::UPickupSpawnerController()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPickupSpawnerController::BeginPlay()
{
	Super::BeginPlay();
}


void UPickupSpawnerController::SetupInputs(ACustomCharacter* character_, TObjectPtr<UInputComponent> inputComponent)
{
	character = character_;

	pickupSpawner = character->GetComponentByClass<UPickupSpawner>();
	if (IsValid(pickupSpawner))
	{
		pickupSpawner->SetupCharacter(character);
	}

	inputComponent->BindAction(SpawnStandardInputName, EInputEvent::IE_Pressed, this, &UPickupSpawnerController::SpawnStandard);
	inputComponent->BindAction(SpawnDestroyAfterThrowInputName, EInputEvent::IE_Pressed, this, &UPickupSpawnerController::SpawnDestroyAfterThrow);
	inputComponent->BindAction(SpawnDestroyAfterTakeInputName, EInputEvent::IE_Pressed, this, &UPickupSpawnerController::SpawnDestroyAfterTake);
	inputComponent->BindAction(PrintPickupsInfosInputName, EInputEvent::IE_Pressed, this, &UPickupSpawnerController::PrintPickupsInfos);
}


void UPickupSpawnerController::SpawnStandard()
{
	if (!IsValid(pickupSpawner)) return;

	pickupSpawner->SpawnPickup(EPickupType::Standard);
}

void UPickupSpawnerController::SpawnDestroyAfterThrow()
{
	if (!IsValid(pickupSpawner)) return;

	pickupSpawner->SpawnPickup(EPickupType::DestroyAfterThrow);
}

void UPickupSpawnerController::SpawnDestroyAfterTake()
{
	if (!IsValid(pickupSpawner)) return;

	pickupSpawner->SpawnPickup(EPickupType::DestroyAfterTake);
}

void UPickupSpawnerController::PrintPickupsInfos()
{
	if (!IsValid(pickupSpawner)) return;

	pickupSpawner->PrintPickupInfos();
}