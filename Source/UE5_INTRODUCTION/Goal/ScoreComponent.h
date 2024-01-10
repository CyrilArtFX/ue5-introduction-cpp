#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Goal.h"
#include "ScoreComponent.generated.h"


USTRUCT(BlueprintType)
struct FTeamScore
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	EGoalTeam GoalTeam{ EGoalTeam::None };

	UPROPERTY(VisibleAnywhere)
	int score{ 0 };
};


UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UE5_INTRODUCTION_API UScoreComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UScoreComponent();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere)
	TMap<EGoalTeam, FTeamScore> scores
	{
		{EGoalTeam::Green, FTeamScore{EGoalTeam::Green, 0}},
		{EGoalTeam::Orange, FTeamScore{EGoalTeam::Orange, 0}}
	};


	UFUNCTION()
	void OnGoalScored(EGoalTeam goalTeam);


public:
	void DisplayScores();
};
