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

	if (IsValid(gravityGun) && gravityGun->UseComplexThrowForce())
	{
		inputComponent->BindAction(ThrowInputName, EInputEvent::IE_Pressed, this, &UGravityGunController::ComplexThrowForce);
		inputComponent->BindAction(ThrowInputName, EInputEvent::IE_Released, this, &UGravityGunController::ThrowObjectPressed);
	}
	else
	{
		inputComponent->BindAction(ThrowInputName, EInputEvent::IE_Pressed, this, &UGravityGunController::ThrowObjectPressed);
	}

	inputComponent->BindAxis(RangeInputName, this, &UGravityGunController::RangeChange);

	inputComponent->BindAction(DestroyPickupInHandInputName, EInputEvent::IE_Pressed, this, &UGravityGunController::DestroyPickupInHand);
}

void UGravityGunController::TakeObjectPressed()
{
	if (!IsValid(gravityGun)) return;

	gravityGun->TakeObject();
}

void UGravityGunController::ComplexThrowForce()
{
	if (!IsValid(gravityGun)) return;

	gravityGun->ComplexThrowForceAccumulation();
}

void UGravityGunController::ThrowObjectPressed()
{
	if (!IsValid(gravityGun)) return;

	gravityGun->ThrowObject();
}

void UGravityGunController::RangeChange(float value)
{
	if (!IsValid(gravityGun)) return;
	if (value == 0.0f) return;

	gravityGun->UpdateRange(value * RangeChangeSensitivity);
}

void UGravityGunController::DestroyPickupInHand()
{
	if (!IsValid(gravityGun)) return;

	gravityGun->DestroyPickupInHand();
}
