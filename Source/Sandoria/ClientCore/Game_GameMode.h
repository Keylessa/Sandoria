// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Sandoria/StructsAndEnums/GameTypes.h"
#include "Sandoria/ClientCore/GamePlayerController.h"
#include "Game_GameMode.generated.h"

/**
 * 

*/
UCLASS()
class SANDORIA_API AGame_GameMode : public AGameMode
{
	GENERATED_BODY()

	AGame_GameMode();

	void BeginPlay() override;
public:
	UFUNCTION(BlueprintCallable, Category = "Player")
	void SpawnPlayerInWorld(int32 PlayerID, int32 CharacterID, FVector SpawnLocation);

	UPROPERTY(EditDefaultsOnly, Category = "Character")
	TSubclassOf<APawn> BP_BaseCharacterClass;

	//void UpdateAllPlayers();

};
