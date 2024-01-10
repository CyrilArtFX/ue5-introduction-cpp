#include "Goal/Goal.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Pickup/Pickup.h"

AGoal::AGoal(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionBox = ObjectInitializer.CreateOptionalDefaultSubobject<UBoxComponent>(this, TEXT("Collision Box"));
	if (IsValid(CollisionBox))
	{
		SetRootComponent(CollisionBox);
	}
}

void AGoal::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(CollisionBox))
	{
		CollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &AGoal::OnGoalOverlap);
	}
}

void AGoal::OnGoalOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APickup* pickup = Cast<APickup>(OtherActor);

	if (!IsValid(pickup)) return;

	pickup->StopVelocity();

	OnGoalScore.Broadcast(GoalTeam);
}

int AGoal::CountPickups()
{
	int score = 0;

	FVector goal_location = GetActorLocation();
	FRotator goal_rotation = GetActorRotation();
	FVector scale_box_extent = CollisionBox->GetScaledBoxExtent();

	const TArray<AActor*> actors_to_ignore;
	TArray<FHitResult> hit_results;

	UKismetSystemLibrary::BoxTraceMulti(this, goal_location, goal_location, scale_box_extent, goal_rotation, GoalTraceChannel, false, actors_to_ignore,
		EDrawDebugTrace::None, hit_results, true);

	for (auto& hit_result : hit_results)
	{
		APickup* pickup = Cast<APickup>(hit_result.GetActor());
		if (!IsValid(pickup)) continue;

		score++;
	}


	return score;
}