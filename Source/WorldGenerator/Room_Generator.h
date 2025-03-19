// Copyright Andre Mossi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Master_Room.h"
#include "Props.h"
#include "GameFramework/Actor.h"
#include "Room_Generator.generated.h"

UCLASS()
class WORLDGENERATOR_API ARoom_Generator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoom_Generator();

	/**
	 * Destroy current generated room.
	 *
	 * @return void
	 */
	void DeleteGeneratedRooms();

	/**
	 * Deletes current generated rooms and started new one.
	 *
	 * @return void
	 */
	void RebootGeneratedRooms();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// -----------------------------------------------------------------------
	// Required Fields
	// -----------------------------------------------------------------------


	// List of spawn locations
	UPROPERTY(EditDefaultsOnly)
	USceneComponent* SpawnLocation;

	// List of rooms to spawn
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AMaster_Room>> RoomToSpawn;

	// List of actors to spawn
	UPROPERTY(EditDefaultsOnly)
	TArray<FProps> props;

	// Max amount of rooms to spawn
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0))
	int maxRooms;

	// Minimum amount of rooms to spawn
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0))
	int minRooms;
	
	/**
	 * Seed to generate the rooms
	 *
	 * @note
	 *	If the seed equals -1 then it will return a randomly generated world
	*/
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = -1))
	int seed = -1;

	// Seed stream
	FRandomStream stream;


	// -----------------------------------------------------------------------
	// Prototypes
	// -----------------------------------------------------------------------

	/**
	 * Start generating the rooms
	 * @pre Set your room actors on the blueprint beforehand
	 * @return void
	 */
	void GenerateRooms();

	/**
	 * Spawn desired room in given location.
	 *
	 * @param transform Location to spawn.
	 * @param roomIndex Room selection from list.
	 * @return Spawned actor
	 */
	void SpawnRoom(FTransform transform, int roomIndex);

	/**
	 * Close all the exits from the list.
	 * 
	 * @param propsList The list of exits to close.
	 * @return void
	 */
	void CloseExits(TArray<TObjectPtr<USceneComponent>> propsList);
	
	/**
	 * Spawn all props from the list with respective settings.
	 * 
	 * @param spawnList The list of props locations.
	 * @return void
	 */
	void SpawnProps(TArray<TObjectPtr<USceneComponent>> spawnList);

	
	// -----------------------------------------------------------------------
	//  Fields
	// -----------------------------------------------------------------------

	/**
	 * Check if there is a room connected.
	 *
	 * @param exitsList The list of exits to close.
	 * @param spaceIndex Exit to check
	 * @return true if there is a room connected, else false.
	 */
	bool ConnectedRoom(TArray<TObjectPtr<USceneComponent>> exitsList, int spaceIndex);

	/**
	 * Check if exit is available taking in account the size of the room to spawn.
	 * If it's not then it will look for another possible space. If no spaces fit the room
	 * then remove room from list
	 *
	 * @param exits The list of available exits to use.
	 * @param roomIndex Desired room.
	 * @param spaceIndex Desired exit.
	 * @return -1 if not possible and if possible than new space.
	 */
	int AvailableRandSpace(TArray<TObjectPtr<USceneComponent>> exits, int roomIndex, int spaceIndex);

	// -----------------------------------------------------------------------
	//  List Of Spawned Actors
	// -----------------------------------------------------------------------

	// List of spawned closers of exits
TArray<TObjectPtr<AActor>> spawnedClosers;
	// List of spawned rooms
	TArray<TObjectPtr<AMaster_Room>> spawnedRooms;
	// List of spawned CLosers
	TArray<TObjectPtr<AActor>> spawnedProps;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
