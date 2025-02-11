// Copyright Epic Games, Inc. All Rights Reserved.

#include "SandoriaGameMode.h"
#include "Sandoria/Characters/BaseCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASandoriaGameMode::ASandoriaGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Sandoria/Blueprint/Characters/BP_BaseCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
