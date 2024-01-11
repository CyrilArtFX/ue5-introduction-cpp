#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PickupSpawnerController.generated.h"


class ACustomCharacter;
class UInputComponent;
class UPickupSpawner;


UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5_INTRODUCTION_API UPickupSpawnerController : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPickupSpawnerController();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Inputs|SpawnPickup")
	FName SpawnStandardInputName{};

	UPROPERTY(EditAnywhere, Category = "Inputs|SpawnPickup")
	FName SpawnDestroyAfterThrowInputName{};

	UPROPERTY(EditAnywhere, Category = "Inputs|SpawnPickup")
	FName SpawnDestroyAfterTakeInputName{};

	UPROPERTY(EditAnywhere, Category = "Inputs|Utility")
	FName PrintPickupsInfosInputName{};


protected:
	void SpawnStandard();
	void SpawnDestroyAfterThrow();
	void SpawnDestroyAfterTake();
	void PrintPickupsInfos();


protected:
	ACustomCharacter* character{ nullptr };
	UPickupSpawner* pickupSpawner{ nullptr };


public:
	void SetupInputs(ACustomCharacter* character_, TObjectPtr<UInputComponent> inputComponent);
};
