// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientCore/Game_GameMode.h"
#include "Sandoria/ClientCore/SandoriaGameInstance.h"

AGame_GameMode::AGame_GameMode()
{
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
    /*
    USandoriaGameInstance* GameInstance = Cast<USandoriaGameInstance>(GetWorld()->GetGameInstance());
    if (GameInstance)
    {
        GameInstance->SpawnPlayerPawn();  // Apelează funcția de spawn
    }

    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (!PlayerController)
    {
        UE_LOG(LogTemp, Error, TEXT("Eroare: PlayerController nu a fost găsit!"));
        return;
    }

    APawn* PlayerPawn = PlayerController->GetPawn();
    if (!PlayerPawn)
    {
        UE_LOG(LogTemp, Error, TEXT("Eroare: Pawn-ul jucătorului nu a fost creat!"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Pawn-ul jucătorului a fost creat corect: %s"), *PlayerPawn->GetName());
    } */
}
/*
void AGame_GameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    if (NewPlayer && HasAuthority()) // ✅ Asigură că doar serverul execută acest cod
    {
        // Dacă nu există deja un pawn, creează unul
        if (!NewPlayer->GetPawn())
        {
            UE_LOG(LogTemp, Warning, TEXT("Player pawn is NULL. Spawning new one..."));

            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = NewPlayer;
            SpawnParams.Instigator = NewPlayer->GetPawn();
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

            FVector SpawnLocation = FVector(0, 0, 200);
            FRotator SpawnRotation = FRotator::ZeroRotator;

            // ✅ Asigură că DefaultPawnClass este setat corect
            APawn* NewPawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, SpawnLocation, SpawnRotation, SpawnParams);
            if (NewPawn)
            {
                NewPawn->SetReplicates(true);        // ✅ Asigură replicarea actorului
                NewPawn->SetReplicateMovement(true); // ✅ Replică mișcarea corect
                NewPlayer->Possess(NewPawn);

                UE_LOG(LogTemp, Warning, TEXT("Pawn-ul jucătorului a fost creat corect: %s"), *NewPawn->GetName());
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Eroare la crearea Pawn-ului!"));
            }
        }

        // Notificăm toți jucătorii să actualizeze lista de caractere
        UpdateAllPlayers();
    }
}


void AGame_GameMode::UpdateAllPlayers()
{
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        AGamePlayerController* PlayerController = Cast<AGamePlayerController>(*It);
        if (PlayerController)
        {
            //PlayerController->ClientUpdateCharacterList();
        }
    }
}
*/