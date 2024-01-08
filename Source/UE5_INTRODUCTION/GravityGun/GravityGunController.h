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

protected:
	void TakeObjectPressed();
	void ThrowObjectPressed();


protected:
	ACustomCharacter* character{ nullptr };
	UGravityGunComponent* gravityGun{ nullptr };

public:
	void SetupInputs(ACustomCharacter* character_, TObjectPtr<UInputComponent> inputComponent);
};
