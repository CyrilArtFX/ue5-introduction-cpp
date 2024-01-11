#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Pickup/Pickup.h"
#include "PickupSpawner.generated.h"

class ACustomCharacter;
class UGravityGunComponent;

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UE5_INTRODUCTION_API UPickupSpawner : public UActorComponent
{
	GENERATED_BODY()

public:
	UPickupSpawner();

protected:
	virtual void BeginPlay() override;


public:
	void SetupCharacter(ACustomCharacter* character_);

	void SpawnPickup(EPickupType pickupType);
	void PrintPickupInfos();


protected:
	UFUNCTION()
	void OnPickupDestroyed(EPickupType pickupType);


protected:
	UPROPERTY(EditAnywhere, Category = "PickupSpawner|PickupTypes")
	TSubclassOf<APickup> PickupStandard;

	UPROPERTY(EditAnywhere, Category = "PickupSpawner|PickupTypes")
	TSubclassOf<APickup> PickupDestroyAfterThrow;

	UPROPERTY(EditAnywhere, Category = "PickupSpawner|PickupTypes")
	TSubclassOf<APickup> PickupDestroyAfterTake;

	UPROPERTY(EditAnywhere, Category = "PickupSpawner|Spawn")
	FVector PickupSpawnOffsetFromPlayer{ 200.0f, 0.0f, 0.0f };

	UPROPERTY(EditAnywhere, Category = "PickupSpawner|Spawn", meta = (min = 0.0f, max = 10.0f))
	float DelayBetweenTwoSpawn{ 1.0f };

	UPROPERTY(EditAnywhere, Category = "PickupSpawner|MaxPickupValues", meta = (min = 0, tooltip = 
	"The maximum number of Pickups of types that can be on the stage."))
	TMap<EPickupType, int> MaxNumberPickups
	{
		{ EPickupType::Standard, 40 },
		{ EPickupType::DestroyAfterThrow, 10 },
		{ EPickupType::DestroyAfterTake, 10 }
	};

	UPROPERTY(EditAnywhere, Category = "PickupSpawner|MaxPickupValues", meta = (min = 0, tooltip =
		"The maximum number of Pickups (all types) that can be on the stage."))
	int MaxPickupAllTypes{ 50 };


protected:
	ACustomCharacter* character{ nullptr };
	UGravityGunComponent* gravityGun{ nullptr };

	TMap<EPickupType, int> numberPickups
	{
		{ EPickupType::Standard, 0 },
		{ EPickupType::DestroyAfterThrow, 0 },
		{ EPickupType::DestroyAfterTake, 0 }
	};

	FTimerHandle pickupSpawnDelayTimerHandle{};
	bool bSpawnable{ true };


protected:
	int GetNumberAllTypesPickups();
	void SetSpawnableTrue();
	void ClearTimer();
};
