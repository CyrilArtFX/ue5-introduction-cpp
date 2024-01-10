#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Goal.generated.h"

class UBoxComponent;


UENUM()
enum class EGoalTeam : uint8
{
	None,
	Green,
	Orange,

	MAX			UMETA(Hidden)
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGoalScoreDelegate, EGoalTeam, GoalTeam);


UCLASS()
class UE5_INTRODUCTION_API AGoal : public AActor
{
	GENERATED_BODY()
	
public:	
	AGoal(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;


protected:
	UPROPERTY(EditAnywhere, Category = "Goal|Collision")
	TEnumAsByte<ETraceTypeQuery> GoalTraceChannel{ ETraceTypeQuery::TraceTypeQuery1 };

	UPROPERTY(EditAnywhere, Category = "Goal|Team")
	EGoalTeam GoalTeam{ EGoalTeam::None };


	//  collision box
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UBoxComponent* CollisionBox{ nullptr };

	UFUNCTION()
	void OnGoalOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	int CountPickups();
	inline EGoalTeam GetTeam() { return GoalTeam; }


public:
	FGoalScoreDelegate OnGoalScore;
};
