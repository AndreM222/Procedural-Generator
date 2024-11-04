// Copyright Epic Games, Inc. All Rights Reserved.

#include "AISandboxGameMode.h"
#include "AISandboxCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAISandboxGameMode::AAISandboxGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
