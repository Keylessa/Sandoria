/////////////////////////////
// CharacterCreaterUI.cpp
/////////////////////////////

#include "CreateCharacterUI.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Sandoria/ClientCore/MainMenuGameMode.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void UCreateCharacterUI::NativeConstruct()
{
    Super::NativeConstruct();

    GameInstances = Cast<USandoriaGameInstance>(GetGameInstance());

   
    if (Button_Customize)
    {
        Button_Customize->OnClicked.AddDynamic(this, &UCreateCharacterUI::OnCustomizeClicked);
    }

    if (Button_Back_Customize)
    {
        Button_Back_Customize->OnClicked.AddDynamic(this, &UCreateCharacterUI::OnBackCustomizeClicked);
    }

    if (BackToCharacterSelectionButton)
    {
        BackToCharacterSelectionButton->OnClicked.AddDynamic(this, &UCreateCharacterUI::OnBackToCharSelectClicked);
    }

    if (Button_Finish)
    {
        Button_Finish->OnClicked.AddDynamic(this, &UCreateCharacterUI::OnFinishClicked);
    }

    // Inițial ascundem panelul de personalizare
    if (CustomizePanel)
    {
        CustomizePanel->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UCreateCharacterUI::OnCustomizeClicked()
{
    if (MainPanel && CustomizePanel)
    {
        MainPanel->SetVisibility(ESlateVisibility::Hidden);
        CustomizePanel->SetVisibility(ESlateVisibility::Visible);
    }
}

void UCreateCharacterUI::OnBackCustomizeClicked()
{
    if (MainPanel && CustomizePanel)
    {
        CustomizePanel->SetVisibility(ESlateVisibility::Hidden);
        MainPanel->SetVisibility(ESlateVisibility::Visible);
    }
}

void UCreateCharacterUI::OnBackToCharSelectClicked()
{
    AMainMenuGameMode* GameMode = Cast<AMainMenuGameMode>(UGameplayStatics::GetGameMode(this));
    if (GameMode)
    {
        GameMode->RemoveCharacterCreationPanel();
        GameMode->ShowCharacterSelectionWidget();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Eroare: GameMode nu a fost găsit!"));
    }
}

void UCreateCharacterUI::OnFinishClicked()
{
    if (!GameInstances)
    {
        UE_LOG(LogTemp, Error, TEXT("GameInstance is null!"));
        return;
    }

    if (!NameTextBox || NameTextBox->GetText().IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("Numele caracterului nu poate fi gol!"));
        return;
    }

    UClassButtonUI* SelectedClassButton = GetSelectedButton(UClassButtonUI::AllClassButtons);
    URaceButton* SelectedRaceButton = GetSelectedButton(URaceButton::AllRaceAndFactionButtons);
    URaceButton* SelectedFactionButton = GetSelectedButton(URaceButton::AllRaceAndFactionButtons);
    UGenderButtonUI* SelectedGenderButton = GetSelectedButton(UGenderButtonUI::AllGenderButtons);


    FString CharacterName = NameTextBox->GetText().ToString();
   
    FString SelectedClass = ClassToString(SelectedClassButton->SelectedClass);
    FString SelectedRace = RaceToString(SelectedRaceButton->SelectedRace);
    FString SelectedFaction = FactionToString(SelectedFactionButton->SelectedFaction);
    FString SelectedGender = GenderToString(SelectedGenderButton->SelectedGender);

    if (!GameInstances->IsCharacterNameAvailable(CharacterName))
    {
        UE_LOG(LogTemp, Warning, TEXT("Numele %s este deja folosit!"), *CharacterName);

        if (ErrorMessageText)
        {
            ErrorMessageText->SetText(FText::FromString(TEXT("Numele deja folosit!")));
            ErrorMessageText->SetVisibility(ESlateVisibility::Visible);
        }
        return;
    }

    // Apelăm funcția de creare a caracterului
    bool bCreated = GameInstances->CreateCharacter(
        CharacterName, SelectedFaction, SelectedRace, SelectedClass, SelectedGender
    );

    if (bCreated)
    {
        UE_LOG(LogTemp, Warning, TEXT("Caracterul %s a fost creat cu succes!"), *CharacterName);

        // Opțional: revenim la selecția caracterelor
        AMainMenuGameMode* GameMode = Cast<AMainMenuGameMode>(UGameplayStatics::GetGameMode(this));
        if (GameMode)
        {
            GameMode->RemoveCharacterCreationPanel();
            GameMode->ShowCharacterSelectionWidget();
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Eroare la crearea caracterului!"));

        if (ErrorMessageText)
        {
            ErrorMessageText->SetText(FText::FromString(TEXT("Eroare la crearea caracterului!")));
            ErrorMessageText->SetVisibility(ESlateVisibility::Visible);
        }
    }
}
