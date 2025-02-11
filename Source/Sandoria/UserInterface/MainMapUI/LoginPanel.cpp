////////////////////////////
//LoginPanel.cpp
////////////////////////////


#include "LoginPanel.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Sandoria/ClientCore/MainMenuGameMode.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void ULoginPanel::NativeConstruct()
{
    Super::NativeConstruct();

    // Abonare la evenimentul de click pe butonul de login
    if (LoginButton)
    {
        LoginButton->OnClicked.AddDynamic(this, &ULoginPanel::OnLoginClicked);
    }

    // Ascundem inițial mesajul de eroare/succes
    if (MessageText)
    {
        MessageText->SetVisibility(ESlateVisibility::Hidden);
    }

    // Ascundem widget-ul de selecție a caracterului
    if (CharacterSelectionWidget)
    {
        CharacterSelectionWidget->SetVisibility(ESlateVisibility::Hidden);
    }
}

void ULoginPanel::NativeDestruct()
{
    Super::NativeDestruct();

    if (LoginButton)
    {
        LoginButton->OnClicked.Clear();
    }
}

void ULoginPanel::OnLoginClicked()
{
    // Verificăm dacă UsernameBox și PasswordBox sunt valide
    if (!UsernameBox || !PasswordBox)
    {
        UE_LOG(LogTemp, Error, TEXT("UsernameBox or PasswordBox is null!"));
        return;
    }

    FString Username = UsernameBox->GetText().ToString();
    FString Password = PasswordBox->GetText().ToString();

    // Obținem referința la GameInstance și GameMode
    USandoriaGameInstance* GameInstance = Cast<USandoriaGameInstance>(GetGameInstance());
    AMainMenuGameMode* GameMode = Cast<AMainMenuGameMode>(UGameplayStatics::GetGameMode(this));

    if (GameInstance)
    {
        // Se efectuează autentificarea (acest apel este blocant și se presupune că gestionează conexiunea prin NetworkManager)
        bool bSuccess = GameInstance->Authenticate(Username, Password);

        if (MessageText)
        {
            // Afișăm mesajul
            MessageText->SetVisibility(ESlateVisibility::Visible);

            if (bSuccess)
            {
                UE_LOG(LogTemp, Warning, TEXT("Login Successful!"));

                // Dacă GameMode este valid, eliminăm panoul de login și afișăm widget-ul pentru selecția caracterului
                if (GameMode)
                {
                    GameMode->RemoveLoginPanel();
                    GameMode->ShowCharacterSelectionWidget();
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("GameMode nu a fost găsit!"));
                }
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Login Failed!"));
                // Dacă dorești, poți trimite un mesaj către server pentru eroare (comentat aici pentru claritate)
                // FString LoginDataToSend = "LOGIN:" + Username + ":" + "Fail! Reason: Incorrect username or password!";
                // GameInstance->SendData(LoginDataToSend);

                MessageText->SetText(FText::FromString(TEXT("Incorrect username or password!")));
                MessageText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
            }

            // Ascundem mesajul după 5 secunde
            GetWorld()->GetTimerManager().SetTimer(VisibilityTimerHandle, [this]()
                {
                    if (MessageText)
                    {
                        MessageText->SetVisibility(ESlateVisibility::Hidden);
                    }
                }, 5.0f, false);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("GameInstance nu a fost găsit!"));
    }
}

