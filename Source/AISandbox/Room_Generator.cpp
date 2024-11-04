// Copyright Andre Mossi. All Rights Reserved.

#include "Room_Generator.h"

#include "Master_Room.h"

// Sets default values
ARoom_Generator::ARoom_Generator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Required Scene Components
	SpawnLocation = CreateDefaultSubobject<USceneComponent>("SpawnLocation");
}

// Called when the game starts or when spawned
void ARoom_Generator::BeginPlay()
{
	Super::BeginPlay();

	GenerateRooms();
}


// Called every frame
void ARoom_Generator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARoom_Generator::SpawnRoom(FTransform transform, int roomIndex)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	spawnedRooms.Add(GetWorld()->SpawnActor<AMaster_Room>( // Spawn actor
		RoomToSpawn[roomIndex],
		transform.GetLocation(),
		transform.Rotator(),
		SpawnParams
	));
}

void ARoom_Generator::GenerateRooms()
{
	// AMaster_Room* room;
	TArray<TObjectPtr<USceneComponent>> exits;
	TArray<TObjectPtr<USceneComponent>> spawners;

	if(seed > -1) stream.Initialize(seed);
	else stream.GenerateNewSeed();

	if (minRooms > maxRooms) maxRooms = minRooms; // keep min same or lower than max

	if(RoomToSpawn.IsEmpty()) return; // Stop if no rooms set for spawn

	for (int i = 0; i < maxRooms; i++)
	{
		int spaceIndex;// Random Arrow
		int roomIndex; // Random Room

		do
		{
			spaceIndex = stream.RandRange(0, exits.Num() - 1);
			roomIndex = stream.RandRange(0, RoomToSpawn.Num() - 1);
			
			spaceIndex = AvailableRandSpace(exits, roomIndex, spaceIndex);
		} while (spaceIndex < 0 && !RoomToSpawn.IsEmpty()); // Check For Possible Room and Space

		if (RoomToSpawn.IsEmpty()) break;
		
		if (!exits.IsEmpty()) // Spawn Following Rooms
		{
			//Set Transformation
			FTransform exitLocation = exits[spaceIndex]->GetComponentTransform();
			FTransform actorLocation = exits[spaceIndex]->GetAttachParentActor()->GetActorTransform();

			exitLocation.SetLocation(
				exitLocation.GetLocation() + (exitLocation.GetLocation() - actorLocation.GetLocation())
			);

			SpawnRoom(exitLocation, roomIndex); // Spawn Room
			exits.Remove(*exits[spaceIndex]); // Delete Used Space
		}
		
		if (spawnedRooms.IsEmpty()) // Spawn starting room
			SpawnRoom(
				FTransform(FRotator::ZeroRotator, SpawnLocation->GetComponentLocation()),
				roomIndex
			);
		
		if(spawnedRooms.Num() > i)
		{
			exits.Append(spawnedRooms[i]->exitArrows->GetAttachChildren()); // Add New Exits
			spawners.Append(spawnedRooms[i]->spawners->GetAttachChildren()); // Add New Exits
		}
	}
	
	// No more rooms to spawn, start from scratch
	if (spawnedRooms.Num() < minRooms && seed == -1)
	{
		RebootGeneratedRooms();
		return;
	}
	
	CloseExits(exits);
	SpawnProps(spawners);
}

int ARoom_Generator::AvailableRandSpace(TArray<TObjectPtr<USceneComponent>> exits, int roomIndex, int spaceIndex)
{
	if (exits.IsEmpty()) return spaceIndex;

	FHitResult Hit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(exits[spaceIndex]->GetAttachParentActor()); // Ignore current actor

	FTransform exitLocation = exits[spaceIndex]->GetComponentTransform(); // Exit Transform
	FTransform actorLocation = exits[spaceIndex]->GetAttachParentActor()->GetActorTransform(); // Actor Transform

	// Spawn temporary room to get its dimensions
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* roomModel = GetWorld()->SpawnActor<AMaster_Room>(
		RoomToSpawn[roomIndex],
		exitLocation.GetLocation(),
		FRotator::ZeroRotator,
		SpawnParams
	);

	// Setup Box Dimensions for trace
	FVector BoxDimensions = FVector(roomModel->GetComponentsBoundingBox().GetExtent() - 4);
	BoxDimensions.Z += 3;
	roomModel->Destroy(); // Destroy tmp room after setup

	FVector traceLocation = exitLocation.GetLocation() + (exitLocation.GetLocation() - actorLocation.GetLocation());
	traceLocation.Z += BoxDimensions.Z; // Center Box Trace

	// Trace box to check if collision available
	FCollisionShape box = FCollisionShape::MakeBox(BoxDimensions);
	exits[spaceIndex]->GetWorld()->SweepSingleByChannel(
		Hit,
		traceLocation,
		traceLocation,
		FQuat::Identity,
		ECC_Visibility,
		box,
		QueryParams
	);

	if (Hit.GetActor()) // If hit then find new space
	{
		for (int i = 0; i < exits.Num(); i++)
		{
			traceLocation = exitLocation.GetLocation() + (exitLocation.GetLocation() - actorLocation.GetLocation());
			traceLocation.Z += BoxDimensions.Z; // Center Box Trace

			// Trace box to check if collision available
			exits[i]->GetWorld()->SweepSingleByChannel(
				Hit,
				traceLocation,
				traceLocation,
				FQuat::Identity,
				ECC_Visibility,
				box,
				QueryParams
			);

			if (Hit.GetActor() == nullptr) return i; // Return available space
		}

		RoomToSpawn.Remove(*RoomToSpawn[roomIndex]); // remove since no possible places for this object
		return -1; // If hit than not possible to place and return -1
	}

	return spaceIndex; // If space available then return true
}

void ARoom_Generator::CloseExits(TArray<TObjectPtr<USceneComponent>> propsList)
{
    for (int i = 0; i < propsList.Num(); i++) // Iterate through exits
    {
    	// Get closers available
	    TArray<TSubclassOf<AActor>> closers = Cast<AMaster_Room>(propsList[i]->GetAttachParentActor())->ClosingActors;

    	// If closer available then place it
	    if (closers.Num() > 0 && !ConnectedRoom(propsList, i))
	    {
		    int closerIndex = stream.RandRange(0, closers.Num() - 1); // Random closer
	    	
		    FActorSpawnParameters SpawnParams;
		    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		    FTransform exitLocation = propsList[i]->GetComponentTransform();

		     spawnedClosers.Add(GetWorld()->SpawnActor<AActor>(
			    closers[closerIndex],
			    exitLocation.GetLocation(),
			    exitLocation.Rotator(),
			    SpawnParams
		    ));
	    }
    }

    propsList.Empty(); // Remove exit from list
}

void ARoom_Generator::SpawnProps(TArray<TObjectPtr<USceneComponent>> spawnerList)
{
    for (int i = 0; i < props.Num() && spawnerList.Num(); i++) // Iterate through props
    {
	    for (int j = 0; j < props[i].total && spawnerList.Num(); j++) // Iterate the total amount asked
	    {
		    int spawnIndex = stream.RandRange(0, spawnerList.Num() - 1); // Random spawner

	    	if(!props[i].filter.IsEmpty())
	    	{
	    		TArray<TObjectPtr<USceneComponent>> filteredList; // List of filtered possible ones

			    for (int room = 0; room < spawnerList.Num(); room++)
			    {
				    AMaster_Room* roomActor = Cast<AMaster_Room>(spawnerList[room]->GetAttachParentActor());
				    if (props[i].filter.Contains(roomActor->GetClass()) != props[i].reverse)
					    filteredList.Add(spawnerList[room]); // Add filtered options
			    }

	    		if(filteredList.IsEmpty()) break; // Not possible to spawn anywhere

			    int filteredIndex = stream.RandRange(0, filteredList.Num() - 1); // Random spawner
	    		
			    // Return randomly chosen from filtered options
			    spawnIndex = spawnerList.Find(filteredList[filteredIndex]);
		    }
	    		

		    FActorSpawnParameters SpawnParams;
		    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		    FTransform spawnLocation = spawnerList[spawnIndex]->GetComponentTransform();

		    spawnedProps.Add(GetWorld()->SpawnActor<AActor>(
			    props[i].prop,
			    spawnLocation.GetLocation(),
			    spawnLocation.Rotator(),
			    SpawnParams
		    ));

		    spawnerList.Remove(*spawnerList[spawnIndex]);
	    }
    }

    props.Empty(); // Remove Props From List
}

bool ARoom_Generator::ConnectedRoom(TArray<TObjectPtr<USceneComponent>> exitsList, int spaceIndex)
{
	if (!exitsList.IsEmpty())
	{
		FHitResult Hit;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(exitsList[spaceIndex]->GetAttachParentActor()); // Ignore current actor

		FTransform exitLocation = exitsList[spaceIndex]->GetComponentTransform(); // Exit Transform

		// Set a small box for looking for connections
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// Setup Box Dimensions for trace
		FVector BoxDimensions = FVector(50, 50, 50);

		FVector traceLocation = exitLocation.GetLocation();

		// Trace box to check if collision available
		FCollisionShape box = FCollisionShape::MakeBox(BoxDimensions);
		exitsList[spaceIndex]->GetWorld()->SweepSingleByChannel(
			Hit,
			traceLocation,
			traceLocation,
			FQuat::Identity,
			ECC_Visibility,
			box,
			QueryParams
		);

		// Return true if room type has been found connected
		if (Cast<AMaster_Room>(Hit.GetActor())) return true;
	}

	return false; // If no connection then return false.
}

void ARoom_Generator::DeleteGeneratedRooms()
{
	// Delete all rooms spawned
	for (AMaster_Room* room : spawnedRooms) room->Destroy();

	spawnedRooms.Empty();

	// Delete all walls spawned
	for (AActor* wall : spawnedClosers) wall->Destroy();

	spawnedClosers.Empty();

	// Delete all props spawned
	for (AActor* prop : spawnedProps) prop->Destroy();
	
	spawnedProps.Empty();

	Destroy();
}

void ARoom_Generator::RebootGeneratedRooms()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Delete Old Room Generator
    DeleteGeneratedRooms();
	
	// Setup New Room Generator
	GetWorld()->SpawnActor<ARoom_Generator>( // Spawn actor
		this->GetClass(),
		this->GetActorLocation(),
		this->GetActorRotation(),
		SpawnParams
	);
}