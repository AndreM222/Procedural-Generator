// Copyright Andre Mossi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Master_Room.generated.h"

UCLASS()
class WORLDGENERATOR_API AMaster_Room : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AMaster_Room();
	
	// Exit spawn folder
	UPROPERTY(EditAnywhere)
	USceneComponent* exitArrows;

	// Actor spawner folder
	UPROPERTY(EditAnywhere)
	USceneComponent* spawners;

	// Closing actors
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AActor>> ClosingActors;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Root Component
	UPROPERTY(VisibleAnywhere)
	USceneComponent* roomSetup;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
