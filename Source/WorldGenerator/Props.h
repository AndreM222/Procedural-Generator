// Copyright Andre Mossi. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Props.generated.h"

/**
 * Props to spawn Data
 */
USTRUCT(BlueprintType)
struct FProps
{
	// GENERATED_BODY()
	GENERATED_USTRUCT_BODY()
	
	// Prop to spawn
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> prop;
	
	// Total to spawn
	UPROPERTY(EditAnywhere)
	int total;

	// Spawn only in specific room
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AActor>> filter;

	// If reverse then spawn anywhere but the ones in the filter list
	UPROPERTY(EditAnywhere)
	bool reverse;
};