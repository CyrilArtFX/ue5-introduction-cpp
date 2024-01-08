// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/CustomController.h"
#include "Character/CustomCharacter.h"
#include "Defines.h"


void ACustomController::MoveForward(float value)
{
	kPRINT_TICK("MoveForward : " + FString::SanitizeFloat(value));

	if (!IsValid(character)) return;
	if (value == 0.0f) return;

	character->AddMovementInput(character->GetActorForwardVector(), value);
}

void ACustomController::MoveRight(float value)
{
	kPRINT_TICK("MoveRight : " + FString::SanitizeFloat(value));

	if (!IsValid(character)) return;
	if (value == 0.0f) return;

	character->AddMovementInput(character->GetActorRightVector(), value);
}

void ACustomController::Jump()
{
	kPRINT("Jump");

	if (!IsValid(character)) return;

	character->Jump();
}

void ACustomController::AddPitchInput(float value)
{
	Super::AddPitchInput(value * MouseSensitivityUp);
}

void ACustomController::AddYawInput(float value)
{
	Super::AddYawInput(value * MouseSensitivityRight);
}


void ACustomController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis(MoveForwardInputName, this, &ACustomController::MoveForward);
	InputComponent->BindAxis(MoveRightInputName, this, &ACustomController::MoveRight);
	InputComponent->BindAction(JumpInputName, EInputEvent::IE_Pressed, this, &ACustomController::Jump);

	InputComponent->BindAxis(LookUpInputName, this, &ACustomController::AddPitchInput);
	InputComponent->BindAxis(LookRightInputName, this, &ACustomController::AddYawInput);
}

void ACustomController::SetPawn(APawn* pawn)
{
	Super::SetPawn(pawn);

	character = Cast<ACustomCharacter>(pawn);
}
