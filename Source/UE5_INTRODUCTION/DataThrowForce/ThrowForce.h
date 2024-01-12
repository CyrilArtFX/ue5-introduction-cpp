#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ThrowForce.generated.h"


UCLASS()
class UE5_INTRODUCTION_API UThrowForce : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "ThrowForce")
	float MinThrowForce{ 500.0f };

	UPROPERTY(EditAnywhere, Category = "ThrowForce")
	float MaxThrowForce{ 5000.0f };

	UPROPERTY(EditAnywhere, Category = "ThrowForce")
	float TimeToMaxForce{ 3.0f };
};
