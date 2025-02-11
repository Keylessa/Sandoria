// ==============================
// SandoriaGameInstance.cpp
// ==============================

#include "SandoriaGameInstance.h"  
#include "UserInterface/MainMapUI/LoginPanel.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

USandoriaGameInstance* USandoriaGameInstance::GetInstance()
{
    return Cast<USandoriaGameInstance>(GWorld->GetGameInstance());
}

void USandoriaGameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
    // Poți adăuga aici comportamentul dorit în caz de eșec de rețea.
    UE_LOG(LogTemp, Error, TEXT("GameInstance - Network failure: %s"), *ErrorString);
}

// ==============================
// Inițializare și Shutdown
// ==============================

void USandoriaGameInstance::Init()
{
    Super::Init();

    // Obține instanța Network Manager-ului (subsystem-ul de rețea)
    NetworkManager = NewObject<UNetworkManager>(this, UNetworkManager::StaticClass());
    if (NetworkManager)
    {
        // (Opțional) Abonează-te la evenimentele de rețea.
        if (GEngine)
        {
            GEngine->OnNetworkFailure().AddUObject(NetworkManager, &UNetworkManager::OnNetworkFailure);
        }
        // Alte inițializări legate de rețea pot fi efectuate aici
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("NetworkManager nu a fost găsit în GameInstance!"));
    }
}

void USandoriaGameInstance::Shutdown()
{
    Super::Shutdown();
}

// ==============================
// Autentificare și Gestionare Cont
// ==============================

bool USandoriaGameInstance::Authenticate(const FString& Username, const FString& Password)
{
    // Obținem pointerul către Network Manager
    NetworkManager = GetSubsystem<UNetworkManager>(this);
    if (!NetworkManager)
    {
        UE_LOG(LogTemp, Error, TEXT("Authenticate: NetworkManager nu a fost găsit!"));
        return false;
    }

    // Conectare la server
    if (!NetworkManager->ConnectToServer("127.0.0.1", 12345))
    {
        UE_LOG(LogTemp, Error, TEXT("Authenticate: ConnectToServer a eșuat."));
        return false;
    }

    FString DataToSend = "LOGIN:" + Username + ":" + Password;
    if (!NetworkManager->SendData(DataToSend))
    {
        UE_LOG(LogTemp, Error, TEXT("Authenticate: SendData a eșuat."));
        NetworkManager->DisconnectFromServer();
        return false;
    }

    FString Response = NetworkManager->ReceiveData();
    NetworkManager->DisconnectFromServer();

    // Verifică dacă răspunsul primit este valid
    TArray<FString> ParsedData;
    Response.ParseIntoArray(ParsedData, TEXT(":"), true);

    if (ParsedData.Num() >= 2 && ParsedData[0] == "SUCCESS")
    {
        AccountID = ParsedData[1];
        SetAccountID(AccountID);
        SetAccountName(Username);
        UE_LOG(LogTemp, Warning, TEXT("Autentificare reușită! AccountID: %s"), *AccountID);
        return true;
    }

    UE_LOG(LogTemp, Error, TEXT("Login Failed!"));
    return false;
}

// ==============================
// Gestionare Caracter
// ==============================

TArray<FCharacterStats> USandoriaGameInstance::GetCharacterData()
{
    TArray<FCharacterStats> CharacterList;

    // Dacă nu există un cont logat, nu se poate solicita lista de caractere.
    if (AccountsName.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("No account logged in!"));
        return CharacterList;
    }

    NetworkManager = GetSubsystem<UNetworkManager>(this);
    if (!NetworkManager)
    {
        UE_LOG(LogTemp, Error, TEXT("GetCharacterData: NetworkManager nu a fost găsit!"));
        return CharacterList;
    }

    if (!NetworkManager->ConnectToServer("127.0.0.1", 12345))
    {
        UE_LOG(LogTemp, Error, TEXT("GetCharacterData: Failed to connect to server for character list."));
        return CharacterList;
    }

    FString DataToSend = "GET_CHARACTERS:" + AccountID;
    if (!NetworkManager->SendData(DataToSend))
    {
        UE_LOG(LogTemp, Error, TEXT("GetCharacterData: Failed to send character list request."));
        NetworkManager->DisconnectFromServer();
        return CharacterList;
    }

    FString Response = NetworkManager->ReceiveData();
    NetworkManager->DisconnectFromServer();

    // Eliminăm prefixul "CHAR_LIST:" dacă este prezent
    if (Response.StartsWith("CHAR_LIST:"))
    {
        Response = Response.RightChop(10);        
    }

    if (Response == "NO_CHARACTERS")
    {
        UE_LOG(LogTemp, Warning, TEXT("No characters found for account %s"), *AccountID);
        return CharacterList;
    }

    TArray<FString> CharacterEntries;
    Response.ParseIntoArray(CharacterEntries, TEXT(";"), true);

    for (const FString& Entry : CharacterEntries)
    {
        TArray<FString> CharacterData;
        Entry.ParseIntoArray(CharacterData, TEXT(","), true);

        if (CharacterData.Num() >= 2)  // Verificăm dacă avem minim nume și level
        {
            FCharacterStats NewCharacter;
            NewCharacter.Name = CharacterData[0];
            NewCharacter.CharacterLevel = FCString::Atoi(*CharacterData[1]); // Convertim level-ul

            CharacterList.Add(NewCharacter);
            
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Invalid character data: %s"), *Entry);
        }
    }

    return CharacterList;
}

bool USandoriaGameInstance::CreateCharacter(const FString& CharacterName, const FString& Faction, const FString& Race, const FString& CharacterClass, const FString& Gender)
{
    NetworkManager = GetSubsystem<UNetworkManager>(this);
    if (!NetworkManager)
    {
        UE_LOG(LogTemp, Error, TEXT("CreateCharacter: NetworkManager nu a fost găsit!"));
        return false;
    }

    if (!NetworkManager->ConnectToServer("127.0.0.1", 12345))
    {
        UE_LOG(LogTemp, Error, TEXT("CreateCharacter: ConnectToServer a eșuat."));
        return false;
    }

    if (AccountID.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("CreateCharacter: AccountID nu este setat!"));
        return false;
    }

    FString DataToSend = "CREATE_CHARACTER:" + AccountID + ":" + CharacterName + ":" + Faction + ":" + Race + ":" + CharacterClass + ":" + Gender;

    if (!NetworkManager->SendData(DataToSend))
    {
        UE_LOG(LogTemp, Error, TEXT("CreateCharacter: SendData a eșuat."));
        NetworkManager->DisconnectFromServer();
        return false;
    }

    FString Response = NetworkManager->ReceiveData();
    NetworkManager->DisconnectFromServer();

    return Response == "CHARACTER_CREATED";
}

void USandoriaGameInstance::DeleteCharacter(FCharacterStats CharacterToDelete)
{
    UE_LOG(LogTemp, Warning, TEXT("Deleting character: %s"), *CharacterToDelete.Name);
    // TODO: Adaugă logica pentru a elimina caracterul din baza de date sau din listă
}

bool USandoriaGameInstance::IsCharacterNameAvailable(const FString& CharacterName)
{
    NetworkManager = GetSubsystem<UNetworkManager>(this);
    if (!NetworkManager)
    {
        UE_LOG(LogTemp, Error, TEXT("IsCharacterNameAvailable: NetworkManager nu a fost găsit!"));
        return false;
    }

    if (!NetworkManager->ConnectToServer("127.0.0.1", 12345))
    {
        UE_LOG(LogTemp, Error, TEXT("IsCharacterNameAvailable: ConnectToServer a eșuat."));
        return false;
    }

    FString DataToSend = "CHECK_NAME:" + CharacterName;
    if (!NetworkManager->SendData(DataToSend))
    {
        UE_LOG(LogTemp, Error, TEXT("IsCharacterNameAvailable: Trimiterea datelor a eșuat."));
        NetworkManager->DisconnectFromServer();
        return false;
    }

    FString Response = NetworkManager->ReceiveData();
    NetworkManager->DisconnectFromServer();

    return Response == "NAME_AVAILABLE";
}

// ==============================
// Previzualizare Caracter
// ==============================

void USandoriaGameInstance::InitCharacterPreview()
{
    if (!GetWorld()) return;

    CharacterPreviewActor = GetWorld()->SpawnActor<AActorCharacterPreviewActor>(AActorCharacterPreviewActor::StaticClass());

    if (CharacterPreviewActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("CharacterPreviewActor a fost creat!"));
    }
}

