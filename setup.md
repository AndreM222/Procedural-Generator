<h1> Generator Configurations </h1>

<!--toc:start-->
- [Spawning](#spawning)
  - [Generator](#generator)
    - [Seed](#seed)
  - [Rooms](#rooms)
  - [Walls](#walls)
  - [Props and Actors](#props-and-actors)
    - [Filter](#filter)
    - [Reverse](#reverse)
    - [Total](#total)
  - [Functions](#functions)
<!--toc:end-->

## Spawning

There are various amount of things that can be spawned in this procedural generator.
Does being the props/actors, walls, and the rooms.

All this options to spawn require the setup of some arrows inside some component scenes
being the followings:

- `Rooms` MasterRoom(Child) -> ExitArrows
    - Only spawns MasterRoom(Type)

- `Walls` MasterRoom(Child) -> ExitArrows
    - Spawns Actor(Type)

- `Props/Actors` MasterRoom(Child) -> Spawners
    - Spawns Actor(Type)

### Generator

Right at the beginning you are require to make a blueprint of type `Room_Generator`.
Once its created you can make children to have different kind of generators based on taste.

![GeneratorPreview](img/GeneratorPreview.png)

You have vairous options for the setup.

- [Room to spawn](#rooms) -> setup rooms to spawn
- [Props](#props-and-actors) -> Spawn of props/actors
- Min/Max Rooms -> Setup the minimum and maximum to spawn.
- [Seed](#seed) -> Seed for spawning

#### Seed

Seed will define if it will be random or not.

To make it random the number must equal `-1`.
If the value is greater, than it will spawn depending on the seed given.

### Rooms

- Before continuing check the setup of the [Generator](#generator)

First you are required to make a blueprint with the type `Master_Room`. This will let us later make
children of the rooms that we will want to use for the generator so it can autogenerate the map.

From there on you will want to make childrens of it which they will be the rooms it will spawn.

![ChildrenRoomSetup](img/MasterRoomChildrenPreview.png)

Once inside you need to create arrows which will indicate where will the following rooms can be spawned.

![ExitsPreview](img/ExitsPreview.png)

For the rooms to be spawned they need to be added into the list of the room generator of
rooms to spawn.

![RoomsToSpawn](img/PreviewRoomSpawn.png)

Once setup the rooms will start spawning on startup

### Walls

- Before continuing check the setup of the [Rooms](#rooms)

When the generator is done it could leave some exits that have not been closed.
For this kind of events it has been added that it will try to put walls on all of the
exits that will make the Players/NPCs to fall off.

But for this to be more customizable I made it so you can specify the type of wall you want.
On purpose it was designed for it to be of type actors in case doors, or other kind of actors
may be desired to use instead of a normal wall.

To set it up tou need to go to `Master_Room(Child)->ClosingActors` and add the closing desired
actor.

![ClosingPreview](img/ClosingActorPreview.png)

You can set up as many as desired. They will place themselves automatically in the exit arrows.

### Props and Actors

- Before continuing check the setup of the [Rooms](#rooms) and [Generator](#generator)

This generator has the ability of spawning actors as well. This was done
in case either some actors or entities might want to be spawned randomly.

Before setting up the desired actors to spawn first we need to set up the places it can spawn in a room.
for the go to `Master_Room(Child)->Spawners` and add arrows inside to give the possible places and rotations
for acotrs/props to spawn.

![SpawnPointsPreview](img/SpawnPointsPreview.png)

To set up some actors/props to spawn randomly you will need to go to `RoomGenerator->Props`
and add the desired props with the desired settings.

![PropsPreview](img/PropsPreview.png)

#### Filter

Because there might be a desire to spawn explicitly in specific rooms, it was added the filter option.
When adding a room into the array it will spawn explicitly on what is inside.

If nothing is placed then it will spawn anywhere.

#### Reverse

It will reverse the filter.

When is checked it will take what is inside of the filter and spawn anywhere except on what is in the list.

#### Total

The total amount that is desired to be spawned.

### Functions

This project has some functions that might be useful for some implementations

- DeleteGeneratedRooms -> Deleted all generated rooms including self.
- RebootGeneratedRooms -> Do all setup again from scratch.
