# Procedural Generator

<p align="center">

![ProceduralExample](docs/img/ProceduralPreviewPic.png)

</p>

## Contents

This project contains a procedural generator I developed for the use of my research on AI in unreal engine.
For more information on how to use it refer to the [Documentation](/docs/setup.md).

## Dependencies

- Unreal Engine 5.4.4

## Capabilities

- [Seeds/Random](/docs/setup.md#generator)
    - The option of either generating the rooms from a seed or to generate a random
        room everytime it run.
    - Can also specify the minimum and maximum rooms to spawn.

> [!WARNING]
> If seed is used then it will try it's best to reach the minimum and maximum
> but if it cant make it then it gives what it could


- [Props/Actor Spawning](docs/setup.md#props-and-actors)
    - Spawn props, actors, etc. at any point desires
    - [Filter](docs/setup.md#filter)
        - You can use filters to specify either which specific rooms
            to spawn in or which ones not to spawn in.
    - Can also specify total amount to spawn.

- [Room Setup](docs/setup.md#rooms)
    - You can set the possible walls to use to close the room.
    - Choose spots on where to spawn actors/props.
    - Choose where can a new room be places in any dimension and amount.

## Scripts Needed

- Master_Room -> The setup of the room.
- Room_Generator -> Generator of procedural map.
- Props -> A struct for the props to spawn information.

## License

This project is open for anyone to use anyway they want.

[LICENSE](LICENSE)
