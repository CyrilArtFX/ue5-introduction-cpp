#include "Goal/ScoreComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Defines.h"

UScoreComponent::UScoreComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UScoreComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGoal::StaticClass(), actors);

	for (auto actor : actors)
	{
		AGoal* goal = Cast<AGoal>(actor);
		if (!IsValid(goal)) continue;

		goal->OnGoalScore.AddUniqueDynamic(this, &UScoreComponent::OnGoalScored);
	}
}

void UScoreComponent::OnGoalScored(EGoalTeam goalTeam)
{
	if (!scores.Contains(goalTeam)) return;

	scores[goalTeam].score++;
}

void UScoreComponent::DisplayScores()
{
	kPRINT("=========================================");
	for (auto team_score : scores)
	{
		kPRINT(UEnum::GetValueAsString(team_score.Value.GoalTeam) + " has taken " + FString::FromInt(team_score.Value.score) + " goals.");
	}

	kPRINT("TEAM SCORES :");
	kPRINT("=========================================");
}
