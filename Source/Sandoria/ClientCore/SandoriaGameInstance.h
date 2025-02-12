// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Sandoria/StructsAndEnums/GameTypes.h"        
#include "Sandoria/ClientCore/NetworkManager.h"
#include "Sandoria/Characters/ActorCharacterPreviewActor.h" 
#include "SandoriaGameInstance.generated.h"

class ULoginPanel;
class UCharacterSelectionWidget;
class UCreateCharacterUI;

UCLASS()
class SANDORIA_API USandoriaGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    // Metodă statică pentru a obține instanța GameInstance (singleton)
    static USandoriaGameInstance* GetInstance();

    UNetworkManager* GetNetworkManager() const { return NetworkManager; };

    // Setare/obținere nume jucător local
    void SetLocalPlayerName(const FString& Name) { LocalPlayerName = Name; }
    const FString& GetLocalPlayerName() const { return LocalPlayerName; }

    // Setare/obținere AccountID
    void SetAccountID(const FString& ID) { AccountID = ID; }
    FString GetAccountID() const { return AccountID; }

    // Setare/obținere nume cont (pentru UI sau alte verificări)
    void SetAccountName(const FString& NewAccountName) { AccountsName = NewAccountName; }
    FString GetAccountName() const { return AccountsName; }

    // Funcții de autentificare și gestionare cont
    bool Authenticate(const FString& Username, const FString& Password);

    // Funcții pentru gestionarea caracterelor
    UFUNCTION(BlueprintCallable, Category = "Characters")
    TArray<FCharacterStats> GetCharacterData();

    UFUNCTION(BlueprintCallable, Category = "Characters")
    bool CreateCharacter(const FString& CharacterName, const FString& Faction, const FString& Race, const FString& CharacterClass, const FString& Gender);

    UFUNCTION(BlueprintCallable, Category = "Character Management")
    void DeleteCharacter(FCharacterStats CharacterToDelete);

    // Funcții legate de nivel și spawn
 

    // Verifică disponibilitatea numelui caracterului
    UFUNCTION(BlueprintCallable, Category = "Characters")
    bool IsCharacterNameAvailable(const FString& CharacterName);

    // Inițializare actor pentru previzualizarea caracterului
    void InitCharacterPreview();
    /*
    void ShowLoginPanel();
    void RemoveLoginPanel();

    void ShowCharacterSelectionWidget();
    void RemoveCharacterSelectionPanel();

    void ShowCharacterCreationPanel();
    void RemoveCharacterCreationPanel();
    */
    // Callback apelat în caz de eșec de rețea
    UFUNCTION()
    void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);
protected:
    virtual void Init() override;
    virtual void Shutdown() override;

private:
    UPROPERTY()
    UNetworkManager* NetworkManager;

    // Variabile de cont
    FString AccountsName;
    FString AccountID;

    // Numele jucătorului local – folosit pentru a evita spawnarea duplicată a personajului local
    FString LocalPlayerName;

public:
    // Alte proprietăți (folosite în Blueprint sau în cod)
    
    
    UPROPERTY()
    FString SelectedCharacter;

    UPROPERTY()
    TMap<FString, APawn*> OtherPlayerMap;

    UPROPERTY()
    AActorCharacterPreviewActor* CharacterPreviewActor;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character")
    TSubclassOf<APawn> BP_BaseCharacterClass;


};
