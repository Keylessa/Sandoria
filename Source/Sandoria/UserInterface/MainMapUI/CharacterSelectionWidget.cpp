////////////////////////////////////
// CharacterSelectionWidget.cpp
////////////////////////////////////

#include "UserInterface/MainMapUI/CharacterSelectionWidget.h"
#include "Sandoria/ClientCore/MainMenuGameMode.h"
#include "CharacterItem.h"
#include "Kismet/GameplayStatics.h"

void UCharacterSelectionWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Configurează butoanele
    if (CreateCharacterButton)
    {
        CreateCharacterButton->OnClicked.AddDynamic(this, &UCharacterSelectionWidget::OnCreateCharacterClicked);
    }

    if (DeleteCharacterButton)
    {
        DeleteCharacterButton->OnClicked.AddDynamic(this, &UCharacterSelectionWidget::OnDeleteCharacterClicked);
    }

    if (BackToLoginButton)
    {
        BackToLoginButton->OnClicked.AddDynamic(this, &UCharacterSelectionWidget::OnBackToLoginClicked);
    }

    if (EnterToWorldButton)
    {
        EnterToWorldButton->OnClicked.AddDynamic(this, &UCharacterSelectionWidget::OnEnterToWorldClicked);
    }

    // Populează lista de caractere
    PopulateCharacterList();
}

void UCharacterSelectionWidget::OnCharacterSelected(UCharacterItem* SelectedWidget)
{
     if (!SelectedWidget)
    {
        UE_LOG(LogTemp, Error, TEXT("SelectedWidget is NULL!"));
        return;
    }

    // Resetăm selecția pentru toate caracterele
    for (UWidget* Widget : CharacterListBox->GetAllChildren())
    {
        UCharacterItem* CharacterWidget = Cast<UCharacterItem>(Widget);
        if (CharacterWidget)
        {
            CharacterWidget->SetSelected(false);
        }
    }

    // Selectăm noul caracter
    SelectedCharacter.Name = SelectedWidget->GetCharacterName();
    SelectedCharacter.CharacterLevel = SelectedWidget->GetCharacterLevel();
    

    SelectedWidget->SetSelected(true);

    UE_LOG(LogTemp, Warning, TEXT("Selected Character: %s"), *SelectedCharacter.Name);

    // Actualizăm actorul de previzualizare
    UpdateCharacterPreview();
}

void UCharacterSelectionWidget::OnCreateCharacterClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("Create Character Clicked!"));
    AMainMenuGameMode* GameMode = Cast<AMainMenuGameMode>(UGameplayStatics::GetGameMode(this));
    if (GameMode)
    {
        GameMode->RemoveCharacterSelectionPanel();
        GameMode->ShowCharacterCreationPanel();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Eroare: GameMode nu a fost găsit!"));
    }
}

void UCharacterSelectionWidget::OnDeleteCharacterClicked()
{
    if (SelectedCharacter.Name.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("No character selected to delete!"));
        return;
    }

    USandoriaGameInstance* GameInstance = Cast<USandoriaGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (!GameInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("GameInstance not found!"));
        return;
    }

    GameInstance->DeleteCharacter(SelectedCharacter);
    PopulateCharacterList();
    UE_LOG(LogTemp, Warning, TEXT("Deleted character: %s"), *SelectedCharacter.Name);
}

void UCharacterSelectionWidget::OnBackToLoginClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("Button Back to Login panel was Clicked!"));

    AMainMenuGameMode* GameMode = Cast<AMainMenuGameMode>(UGameplayStatics::GetGameMode(this));
    if (GameMode)
    {
        GameMode->RemoveCharacterSelectionPanel();
        GameMode->ShowLoginPanel();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Eroare: GameMode nu a fost găsit!"));
    }
}


void UCharacterSelectionWidget::OnEnterToWorldClicked()
{
   
    if (SelectedCharacter.Name.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("No character selected!"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Entering world with: %s"), *SelectedCharacter.Name);

    USandoriaGameInstance* GameInstance = Cast<USandoriaGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (!GameInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("GameInstance not found!"));
        return;
    }

    if (!GameInstance->GetNetworkManager()->IsSocketValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Socket invalid. Încerc să reconectez..."));
        if (!GameInstance->GetNetworkManager()->ConnectToServer("127.0.0.1", 12345))
        {
            UE_LOG(LogTemp, Error, TEXT("Reconectarea la server a eșuat!"));
            return;
        }
    }

    FString RequestData = "ENTER_WORLD:" +
        FString::FromInt(SelectedCharacter.PlayerID) + "," +
        FString::FromInt(SelectedCharacter.CharacterID) + "," +
        FString::SanitizeFloat(SelectedCharacter.PosX) + "," +
        FString::SanitizeFloat(SelectedCharacter.PosY) + "," +
        FString::SanitizeFloat(SelectedCharacter.PosZ) + "\n";

    if (!GameInstance->GetNetworkManager()->SendData(RequestData))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to send Enter World request to server!"));
        return;
    }

    FString Response;
    const float Timeout = 5.0f;
    float ElapsedTime = 0.0f;
    while (ElapsedTime < Timeout)
    {
        Response = GameInstance->GetNetworkManager()->ReceiveMessage();
        GameInstance->GetNetworkManager()->ProcessReceivedMessage(Response);

        if (Response.Contains("ENTER_SUCCESS"))
        {
            break;
        }
        FPlatformProcess::Sleep(0.1f);
        ElapsedTime += 0.1f;
    }

    if (Response.Contains("ENTER_SUCCESS"))
    {
        UE_LOG(LogTemp, Warning, TEXT("Server confirmed character %s entering world."), *SelectedCharacter.Name);
        AMainMenuGameMode* GameMode = Cast<AMainMenuGameMode>(UGameplayStatics::GetGameMode(this));

        // ✅ Eliminăm LoginPanel înainte de schimbarea hărții
        if (GameMode->LoginPanel && GameMode->LoginPanel->IsValidLowLevel())
        {
            GameMode->LoginPanel->RemoveFromParent();
            GameMode->LoginPanel = nullptr;
            UE_LOG(LogTemp, Warning, TEXT("LoginPanel has been removed."));
        }

        // ✅ Eliminăm CharacterSelectionWidget complet
        RemoveFromParent();
        CharacterListBox = nullptr;
        SelectedCharacter = FCharacterStats();
        CharacterPreviewActor = nullptr;

        UE_LOG(LogTemp, Warning, TEXT("Schimbăm harta către MainMap..."));
        UGameplayStatics::OpenLevel(this, "MainMap");
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Enter world failed! Server response: %s"), *Response);
    }
}




void UCharacterSelectionWidget::SelectCharacter(int32 Index)
{
    USandoriaGameInstance* GameInstance = Cast<USandoriaGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (!GameInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("GameInstance not found!"));
        return;
    }

    TArray<FCharacterStats> CharacterList = GameInstance->GetCharacterData();

    if (CharacterList.IsValidIndex(Index))
    {
        SelectedCharacter = CharacterList[Index];
        UE_LOG(LogTemp, Warning, TEXT("Selected character: %s"), *SelectedCharacter.Name);

        // Actualizăm UI-ul pentru a evidenția selecția
        for (UWidget* Widget : CharacterListBox->GetAllChildren())
        {
            UCharacterItem* CharacterWidget = Cast<UCharacterItem>(Widget);
            if (CharacterWidget)
            {
                // Evidențiem caracterul selectat
                CharacterWidget->SetSelected(CharacterWidget->CharacterIndex == Index);              
            }
        }

        // Actualizăm previzualizarea caracterului
        UpdateCharacterPreview();
    }
}

void UCharacterSelectionWidget::UpdateCharacterPreview()
{
    if (!GetWorld())
    {
        UE_LOG(LogTemp, Error, TEXT("No valid world for character preview!"));
        return;
    }

    // Dacă există deja un character preview, îl ștergem
    if (CharacterPreviewActor)
    {
        CharacterPreviewActor->Destroy();
        CharacterPreviewActor = nullptr;
    }

    // Spawnăm un nou actor de previzualizare
    FActorSpawnParameters SpawnParams;
    CharacterPreviewActor = GetWorld()->SpawnActor<AActorCharacterPreviewActor>(AActorCharacterPreviewActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

    if (CharacterPreviewActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("Spawned preview for character: %s"), *SelectedCharacter.Name);
        // Aici poți seta skin-ul sau animațiile în funcție de caracter
    }
}

void UCharacterSelectionWidget::PopulateCharacterList()
{
    if (!CharacterListBox)
    {
        UE_LOG(LogTemp, Warning, TEXT("CharacterListBox is NULL!"));
        return;
    }

    if (!CharacterItemClass)
    {
        UE_LOG(LogTemp, Error, TEXT("CharacterItemClass is NULL! Set it in the editor."));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("PopulateCharacterList() called!"));

    CharacterListBox->ClearChildren();

    USandoriaGameInstance* GameInstance = Cast<USandoriaGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (!GameInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("SandoriaGameInstance not found!"));
        return;
    }

    TArray<FCharacterStats> CharacterList = GameInstance->GetCharacterData();

    if (CharacterList.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No characters found!"));
        return;
    }

    // Creăm widget-urile pentru caractere
    for (int32 i = 0; i < CharacterList.Num(); i++)
    {
        UCharacterItem* CharacterWidget = CreateWidget<UCharacterItem>(this, CharacterItemClass);
        if (!CharacterWidget)
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to create CharacterWidget for %s"), *CharacterList[i].Name);
            continue;
        }

        CharacterWidget->SetCharacterName(CharacterList[i].Name);
        CharacterWidget->SetCharacterLevel(CharacterList[i].CharacterLevel);
        // Setează și alte atribute ale caracterului aici

        // Stocăm indexul caracterului în widget
        CharacterWidget->CharacterIndex = i;

        // Legăm evenimentul de selecție
        CharacterWidget->OnCharacterSelected.BindUObject(this, &UCharacterSelectionWidget::SelectCharacter);

        CharacterListBox->AddChild(CharacterWidget);
        UE_LOG(LogTemp, Warning, TEXT("Widget added for character %s"), *CharacterList[i].Name);
    }

    // 📌 Selectăm automat primul caracter
    SelectCharacter(0);
}
