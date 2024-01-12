// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/CustomController.h"
#include "Character/CustomCharacter.h"
#include "GravityGun/GravityGunController.h"
#include "PickupSpawner/PickupSpawnerController.h"
#include "Goal/Goal.h"
#include "Goal/ScoreComponent.h"
#include "PauseMenu/PauseMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Defines.h"


void ACustomController::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->OnWorldBeginPlay.AddUObject(this, &ACustomController::LateBeginPlay);

	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGoal::StaticClass(), actors);

	for (auto actor : actors)
	{
		goals.Add(CastChecked<AGoal>(actor));
	}
}

void ACustomController::LateBeginPlay()
{
	gravityGunController = GetComponentByClass<UGravityGunController>();

	if (IsValid(gravityGunController))
	{
		gravityGunController->SetupInputs(character, InputComponent);
	}


	pickupSpawnerController = GetComponentByClass<UPickupSpawnerController>();

	if (IsValid(pickupSpawnerController))
	{
		pickupSpawnerController->SetupInputs(character, InputComponent);
	}
}


void ACustomController::MoveForward(float value)
{
	if (!IsValid(character)) return;
	if (value == 0.0f) return;

	character->AddMovementInput(character->GetActorForwardVector(), value);
}

void ACustomController::MoveRight(float value)
{
	if (!IsValid(character)) return;
	if (value == 0.0f) return;

	character->AddMovementInput(character->GetActorRightVector(), value);
}

void ACustomController::Jump()
{
	if (!IsValid(character)) return;

	character->Jump();
}

void ACustomController::Pause()
{
	if (!IsValid(pauseMenuWidget))
	{
		pauseMenuWidget = Cast<UPauseMenuWidget>(CreateWidget(this, PauseMenuClass, "PauseMenu"));
		pauseMenuWidget->AddToViewport(1);
	}
	else
	{
		pauseMenuWidget->OpenMenu();
	}
}

void ACustomController::CountScore()
{
	if (IsValid(scoreComp))
	{
		scoreComp->DisplayScores();
	}

	int total_pickups = 0;

	for (auto goal : goals)
	{
		total_pickups += goal->CountPickups();
	}

	kPRINT("Total number of pickups in all goals is " + FString::FromInt(total_pickups));
}

void ACustomController::AddPitchInput(float value)
{
	Super::AddPitchInput(value * MouseSensitivityUp);
}

void ACustomController::AddYawInput(float value)
{
	Super::AddYawInput(value * MouseSensitivityRight);
}

void ACustomController::SetMouseSensitivityX(float value)
{
	MouseSensitivityRight = value;
}

void ACustomController::SetMouseSensitivityY(float value)
{
	MouseSensitivityUp = value;
}


void ACustomController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis(MoveForwardInputName, this, &ACustomController::MoveForward);
	InputComponent->BindAxis(MoveRightInputName, this, &ACustomController::MoveRight);
	InputComponent->BindAction(JumpInputName, EInputEvent::IE_Pressed, this, &ACustomController::Jump);

	InputComponent->BindAxis(LookUpInputName, this, &ACustomController::AddPitchInput);
	InputComponent->BindAxis(LookRightInputName, this, &ACustomController::AddYawInput);

	FInputActionBinding& score_input_action_binding = InputComponent->BindAction(CountScoreInputName, EInputEvent::IE_Pressed, this, &ACustomController::CountScore);
	score_input_action_binding.bConsumeInput = false;

	InputComponent->BindAction(PauseMenuInputName, EInputEvent::IE_Pressed, this, &ACustomController::Pause);
}

void ACustomController::SetPawn(APawn* pawn)
{
	Super::SetPawn(pawn);

	character = Cast<ACustomCharacter>(pawn);

	if (!IsValid(character)) return;

	scoreComp = character->GetComponentByClass<UScoreComponent>();
}
