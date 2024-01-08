#include "GravityGun/GravityGunController.h"
#include "Character/CustomCharacter.h"
#include "GravityGunComponent.h"
#include "Defines.h"

UGravityGunController::UGravityGunController()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGravityGunController::BeginPlay()
{
	Super::BeginPlay();
}


void UGravityGunController::SetupInputs(ACustomCharacter* character_, TObjectPtr<UInputComponent> inputComponent)
{
	character = character_;

	gravityGun = character->GetComponentByClass<UGravityGunComponent>();
	if (IsValid(gravityGun))
	{
		gravityGun->SetupCharacter(character);
	}

	inputComponent->BindAction(TakeInputName, EInputEvent::IE_Pressed, this, &UGravityGunController::TakeObjectPressed);
	inputComponent->BindAction(ThrowInputName, EInputEvent::IE_Pressed, this, &UGravityGunController::ThrowObjectPressed);
}

void UGravityGunController::TakeObjectPressed()
{
	if (!IsValid(gravityGun)) return;

	gravityGun->TakeObject();
}

void UGravityGunController::ThrowObjectPressed()
{
	if (!IsValid(gravityGun)) return;

	gravityGun->ThrowObject();
}