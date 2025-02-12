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
    // Verificarea UsernameBox și PasswordBox…
    FString Username = UsernameBox->GetText().ToString();
    FString Password = PasswordBox->GetText().ToString();

    USandoriaGameInstance* GameInstance = Cast<USandoriaGameInstance>(GetGameInstance());
    AMainMenuGameMode* GameMode = Cast<AMainMenuGameMode>(UGameplayStatics::GetGameMode(this));

    if (GameInstance)
    {
        bool bSuccess = GameInstance->Authenticate(Username, Password);

        if (MessageText)
        {
            MessageText->SetVisibility(ESlateVisibility::Visible);

            if (bSuccess)
            {
                UE_LOG(LogTemp, Warning, TEXT("Login Successful!"));

                if (GameMode)
                {
                    GameMode->RemoveLoginPanel();
                    GameMode->ShowCharacterSelectionWidget();
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("GameMode nu a fost găsit!"));
                }
                // Nu mai setăm timer-ul dacă login-ul este un succes
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Login Failed!"));
                MessageText->SetText(FText::FromString(TEXT("Incorrect username or password!")));
                MessageText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));

                // Setăm timer-ul doar în cazul eșecului
                GetWorld()->GetTimerManager().SetTimer(VisibilityTimerHandle, [this]()
                    {
                        if (MessageText)
                        {
                            MessageText->SetVisibility(ESlateVisibility::Hidden);
                        }
                    }, 5.0f, false);
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("GameInstance nu a fost găsit!"));
    }
}


