// Copyright Andre Mossi. All Rights Reserved.


#include "Master_Room.h"

// Sets default values
AMaster_Room::AMaster_Room()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Root Component
	roomSetup = CreateDefaultSubobject<USceneComponent>("RoomSetup");
	roomSetup->SetupAttachment(RootComponent);

	// Closing actors
	exitArrows = CreateDefaultSubobject<USceneComponent>("ExitArrows");
	exitArrows->SetupAttachment(roomSetup);

	// Actor spawner folder
	spawners = CreateDefaultSubobject<USceneComponent>("Spawners");
	spawners->SetupAttachment(roomSetup);
}

// Called when the game starts or when spawned
void AMaster_Room::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMaster_Room::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}