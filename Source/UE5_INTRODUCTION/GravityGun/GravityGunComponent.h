#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GravityGunComponent.generated.h"

class ACustomCharacter;
class APlayerCameraManager;

UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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
	void ThrowObject();

protected:
	UPROPERTY(EditAnywhere, Category = "GravityGun|Collision")
	TEnumAsByte<ETraceTypeQuery> GravityGunTraceChannel;
	ECollisionChannel gravityGunCollisionChannel;

	UPROPERTY(EditAnywhere, Category = "GravityGun|Collision", meta = (ClampMin = 0.0f, ClampMax = 5000.0f))
	float GravityGunRange{ 500.0f };

	ACustomCharacter* character{ nullptr };

	TWeakObjectPtr<APlayerCameraManager> cameraManager{ nullptr };
};
