#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GravityGunController.generated.h"

class ACustomCharacter;
class UInputComponent;
class UGravityGunComponent;

UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5_INTRODUCTION_API UGravityGunController : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGravityGunController();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Inputs")
	FName TakeInputName{};

	UPROPERTY(EditAnywhere, Category = "Inputs")
	FName ThrowInputName{};

	UPROPERTY(EditAnywhere, Category = "Inputs")
	FName RangeInputName{};

	UPROPERTY(EditAnywhere, Category = "Inputs", meta = (ClampMin = 1.0f, ClampMax = 100.0f))
	float RangeChangeSensitivity{ 20.0f };

	UPROPERTY(EditAnywhere, Category = "Inputs")
	FName DestroyPickupInHandInputName{};


protected:
	void TakeObjectPressed();
	void ComplexThrowForce();
	void ThrowObjectPressed();
	void RangeChange(float value);
	void DestroyPickupInHand();


protected:
	ACustomCharacter* character{ nullptr };
	UGravityGunComponent* gravityGun{ nullptr };

public:
	void SetupInputs(ACustomCharacter* character_, TObjectPtr<UInputComponent> inputComponent);
};
