#include "Pickup/Pickup.h"

APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = true;

}

void APickup::BeginPlay()
{
	Super::BeginPlay();
	
}

void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickup::ClearTimer()
{
	FTimerManager& timer_manager = this->GetWorldTimerManager();
	timer_manager.ClearTimer(pickupDestructionTimerHandle);
}

void APickup::StartPickupDestructionTimer()
{
	ClearTimer();
	FTimerManager& timer_manager = this->GetWorldTimerManager();
	timer_manager.SetTimer(pickupDestructionTimerHandle, this, &APickup::DestroyPickup, PickupStruct.DestructionTimer, false);
}

void APickup::DestroyPickup()
{
	ClearTimer();

	OnPickupDestroy.Broadcast();

	Destroy();
}

void APickup::StopVelocity()
{
	UStaticMeshComponent* mesh = GetComponentByClass<UStaticMeshComponent>();

	if (!IsValid(mesh)) return;

	mesh->SetAllPhysicsLinearVelocity(FVector::ZeroVector);
	mesh->SetAllPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
}

