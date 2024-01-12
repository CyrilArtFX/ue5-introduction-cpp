// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CustomController.generated.h"

class ACustomCharacter;
class UGravityGunController;
class UPickupSpawnerController;
class AGoal;
class UScoreComponent;
class UPauseMenuWidget;

/**
 *
 */
UCLASS()
class UE5_INTRODUCTION_API ACustomController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void LateBeginPlay();


protected:
	UPROPERTY(EditAnywhere, Category = "Inputs|Movements")
	FName MoveForwardInputName{};

	UPROPERTY(EditAnywhere, Category = "Inputs|Movements")
	FName MoveRightInputName{};

	UPROPERTY(EditAnywhere, Category = "Inputs|Movements")
	FName JumpInputName{};

	UPROPERTY(EditAnywhere, Category = "Inputs|Camera")
	FName LookUpInputName{};

	UPROPERTY(EditAnywhere, Category = "Inputs|Camera")
	FName LookRightInputName{};

	UPROPERTY(EditAnywhere, Category = "Inputs|Camera", meta = (ClampMin = 0.1f, ClampMax = 2.0f))
	float MouseSensitivityUp{ 1.0f };

	UPROPERTY(EditAnywhere, Category = "Inputs|Camera", meta = (ClampMin = 0.1f, ClampMax = 2.0f))
	float MouseSensitivityRight{ 1.0f };

	UPROPERTY(EditAnywhere, Category = "Inputs|Pause")
	FName PauseMenuInputName{};

	UPROPERTY(EditAnywhere, Category = "Inputs|Others")
	FName CountScoreInputName{};

	UPROPERTY(EditAnywhere, Category = "PauseMenu")
	TSubclassOf<UPauseMenuWidget> PauseMenuClass{};


protected:
	void MoveForward(float value);
	void MoveRight(float value);
	void Jump();
	void Pause();
	void CountScore();


public:
	virtual void SetupInputComponent() override;
	virtual void SetPawn(APawn* pawn) override;

	void AddPitchInput(float value) override;
	void AddYawInput(float value) override;

public:
	inline float GetMouseSensitivityX() { return MouseSensitivityRight; }
	inline float GetMouseSensitivityY() { return MouseSensitivityUp; }

	void SetMouseSensitivityX(float value);
	void SetMouseSensitivityY(float value);

protected:
	ACustomCharacter* character{ nullptr };
	UGravityGunController* gravityGunController{ nullptr };
	UPickupSpawnerController* pickupSpawnerController{ nullptr };
	TArray<AGoal*> goals;
	UScoreComponent* scoreComp{ nullptr };
	UPauseMenuWidget* pauseMenuWidget{ nullptr };
};
