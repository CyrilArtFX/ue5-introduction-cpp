// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CustomController.generated.h"

/**
 *
 */
UCLASS()
class UE5_INTRODUCTION_API ACustomController : public APlayerController
{
	GENERATED_BODY()


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

	UPROPERTY(EditAnywhere, Category = "Inputs|Camera")
	float MouseSensitivityUp{ 1.0f };

	UPROPERTY(EditAnywhere, Category = "Inputs|Camera")
	float MouseSensitivityRight{ 1.0f };


protected:
	void MoveForward(float value);
	void MoveRight(float value);
	void Jump();


public:
	virtual void SetupInputComponent() override;
	virtual void SetPawn(APawn* pawn) override;

	void AddPitchInput(float value) override;
	void AddYawInput(float value) override;

protected:
	class ACustomCharacter* character{ nullptr };
};
