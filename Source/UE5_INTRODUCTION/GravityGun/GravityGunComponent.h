#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GravityGunComponent.generated.h"

class ACustomCharacter;
class APlayerCameraManager;
class APickup;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPickupTakenNameDelegate, FString, PickupName);


UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UE5_INTRODUCTION_API UGravityGunComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGravityGunComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetupCharacter(ACustomCharacter* character_);

	void TakeObject();
	void ReleaseObject();
	void ComplexThrowForceAccumulation();
	void ThrowObject();
	void UpdateRange(float change);
	void DestroyPickupInHand();

	void SetObjectInHand(APickup* pickup);

	inline bool UseComplexThrowForce() { return bUseComplexThrowForce; }
	bool HasObjectInHand();

protected:
	UFUNCTION()
	void OnHoldPickupDestroyed();

protected:
	UPROPERTY(EditAnywhere, Category = "GravityGun|Collision")
	TEnumAsByte<ETraceTypeQuery> GravityGunTraceChannel{ ETraceTypeQuery::TraceTypeQuery1 };
	ECollisionChannel gravityGunCollisionChannel{ ECollisionChannel::ECC_EngineTraceChannel1 };

	UPROPERTY(EditAnywhere, Category = "GravityGun|Collision", meta = (ClampMin = 0.0f, ClampMax = 5000.0f))
	float GravityGunRange{ 500.0f };

	UPROPERTY(EditAnywhere, Category = "GravityGun|Collision", meta = (ClampMin = 0.0f, ClampMax = 5000.0f))
	FFloatInterval CustomRange{ 100.0f, 3000.0f };

	ACustomCharacter* character{ nullptr };

	TWeakObjectPtr<APlayerCameraManager> cameraManager{ nullptr };


	//  pickup
protected:
	UPROPERTY(EditAnywhere, Category = "GravityGun|Pickup", meta = (ClampMin = -2000.0f, ClampMax = 2000.0f))
	FVector PickupOffsetFromPlayer{ 200.0f, 0.0f, 0.0f };

	float pickupOffsetXAtMaxCharge{ 0.0f };

	APickup* currentPickup{ nullptr };
	UStaticMeshComponent* pickupMesh{ nullptr };
	FName pickupBaseCollisionProfile{};


	//  throw
protected:
	UPROPERTY(EditAnywhere, Category = "GravityGun|Throw")
	bool bUseComplexThrowForce{ false };
	UPROPERTY(EditAnywhere, Category = "GravityGun|Throw", meta = (ClampMin = 0.0f, ClampMax = 10000.0f, EditCondition = "bUseComplexThrowForce == false", EditConditionHides));
	float PickupThrowForce{ 2000.0f };
	UPROPERTY(EditAnywhere, Category = "GravityGun|Throw", meta = (ClampMin = 0.0f, ClampMax = 15000.0f, EditCondition = "bUseComplexThrowForce == true", EditConditionHides));
	FFloatInterval PickupComplexThrowForce{ 500.0f, 10000.0f };
	UPROPERTY(EditAnywhere, Category = "GravityGun|Throw", meta = (ClampMin = 0.1f, ClampMax = 10.0f, EditCondition = "bUseComplexThrowForce == true", EditConditionHides, Tooltip =
		"The time the player needs to press the input to reach the max throw force"));
	float PickupComplexThrowForceMaxTime{ 2.0f };

	bool bAccumulatingForce{ false };
	float complexForceTimeAccumulated{ 0.0f };


	UPROPERTY(EditAnywhere, Category = "GravityGun|Throw|Angular")
	bool bUseRandomAngularThrow{ false };
	UPROPERTY(EditAnywhere, Category = "GravityGun|Throw|Angular", meta = (EditCondition = "bUseRandomAngularThrow == false", EditConditionHides));
	FVector PickupAngularThrow{ 5000.0f };
	UPROPERTY(EditAnywhere, Category = "GravityGun|Throw|Angular", meta = (ClampMin = 0.0f, EditCondition = "bUseRandomAngularThrow == true", EditConditionHides));
	float RandomAngularThrowRange{ 10000.0f };


public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FPickupTakenNameDelegate OnPickupTaken;


	//  debug
protected:
	UPROPERTY(EditAnywhere, Category = "GravityGun|Debug")
	bool bDrawDebugRaycast{ false };

	UPROPERTY(EditAnywhere, Category = "GravityGun|Debug", meta = (ClampMin = 0.0f, ClampMax = 10.0f), meta = (EditCondition = "bDrawDebugRaycast == true", EditConditionHides))
	float TimeDebugRaycast{ 5.0f };
};
