// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientCore/Game_GameMode.h"
#include "Sandoria/Characters/BaseCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Sandoria/ClientCore/SandoriaGameInstance.h"

AGame_GameMode::AGame_GameMode()
{
    bReplicates = true;
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Sandoria/Blueprint/Characters/BP_BaseCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AGame_GameMode::BeginPlay()
{
	Super::BeginPlay();
}
void AGame_GameMode::SpawnPlayerInWorld(int32 PlayerID, int32 CharacterID, FVector SpawnLocation)
{
    // Se execută doar pe server
    if (!HasAuthority())
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnPlayerInWorld called on client!"));
        return;
    }

    if (!GetWorld())
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnPlayerInWorld: World is null"));
        return;
    }

    if (!DefaultPawnClass)
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnPlayerInWorld: DefaultPawnClass is null!"));
        return;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    SpawnParams.Owner = this;

    // Spawnăm pawn-ul pe server
    ABaseCharacter* NewPlayer = GetWorld()->SpawnActor<ABaseCharacter>(
        DefaultPawnClass,
        SpawnLocation,
        FRotator::ZeroRotator,
        SpawnParams
    );

    if (!NewPlayer)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to spawn player %d (CharacterID %d)"), PlayerID, CharacterID);
        return;
    }

    // Setăm ID-ul jucătorului în pawn (exemplu, pentru replicare sau log)
    NewPlayer->SetPlayerID(PlayerID);
    UE_LOG(LogTemp, Warning, TEXT("Spawned player %d (CharacterID %d) at %s"),
        PlayerID, CharacterID, *SpawnLocation.ToString());

    // Asociem PlayerController-ul cu noul pawn
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, PlayerID);
    if (!PC)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to find PlayerController for PlayerID %d"), PlayerID);
        return;
    }

    // Asigură-te că noul pawn este posedat de controller (aceasta se face pe server)
    PC->UnPossess();
    PC->Possess(NewPlayer);
    UE_LOG(LogTemp, Warning, TEXT("PlayerController %d now possesses the character."), PlayerID);
}
