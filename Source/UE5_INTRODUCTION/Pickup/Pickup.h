#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"


UENUM()
enum class EPickupType : uint8
{
	Standard,
	DestroyAfterThrow,
	DestroyAfterTake,

	MAX			UMETA(Hidden)
};


USTRUCT(BlueprintType)
struct FPickupStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EPickupType PickupType{ EPickupType::Standard };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0.0f, ClampMax = 30.0f, EditCondition = "PickupType == EPickupType::DestroyAfterThrow || PickupType == EPickupType::DestroyAfterTake", EditConditionHides))
	float DestructionTimer{ 5.0f };
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPickupDestroyDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickupDestroyWithTypeDelegate, EPickupType, PickupType);

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UE5_INTRODUCTION_API APickup : public AActor
{
	GENERATED_BODY()

public:
	APickup();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;


protected:
	UPROPERTY(EditAnywhere, Category = "Pickup")
	FPickupStruct PickupStruct{};

public:
	inline EPickupType GetPickupType() { return PickupStruct.PickupType; }

	void ClearTimer();
	void StartPickupDestructionTimer();
	void DestroyPickup();

	FOnPickupDestroyDelegate OnPickupDestroy;
	FOnPickupDestroyWithTypeDelegate OnPickupDestroyWithType;

protected:
	FTimerHandle pickupDestructionTimerHandle{};

public:
	void StopVelocity();

protected:
	UFUNCTION()
	void OnActorDestroyed(AActor* destroyedActor);


private:
	bool bOnPickupDestroyAlreadyBroadcasted{ false };
};
