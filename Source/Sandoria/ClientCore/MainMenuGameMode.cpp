// MainMenuGameMode.cpp


#include "ClientCore/MainMenuGameMode.h"
#include "ClientCore/SandoriaGameInstance.h"



AMainMenuGameMode::AMainMenuGameMode()
{
	// Clasa de UI trebuie setată din Blueprint sau inițializată manual
	LoginPanel = nullptr;

}

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();



	ShowLoginPanel();
}

void AMainMenuGameMode::ShowLoginPanel()
{
	if (LoginPanelClass)
	{
		LoginPanel = CreateWidget<UUserWidget>(GetWorld(), LoginPanelClass);
		if (LoginPanel && LoginPanel->IsValidLowLevel()) {
			if (LoginPanel)
			{
				LoginPanel->AddToViewport();
				LoginPanel->SetVisibility(ESlateVisibility::Visible);
				UE_LOG(LogTemp, Warning, TEXT("LoginPanel a fost adăugat în Viewport!"));
			}
		}
	}
}

void AMainMenuGameMode::RemoveLoginPanel()
{
	if (LoginPanel)
	{
		LoginPanel->RemoveFromParent();
		//LoginPanel = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("LoginPanel a fost ascuns!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Eroare: LoginPanel este nullptr!"));
	}
}

void AMainMenuGameMode::ShowCharacterSelectionWidget()
{
	if (CharacterSelectionPanelClass)
	{
		CharacterSelectionPanel = CreateWidget<UUserWidget>(GetWorld(), CharacterSelectionPanelClass);
		if (CharacterSelectionPanel)
		{
			CharacterSelectionPanel->AddToViewport();
			CharacterSelectionPanel->SetVisibility(ESlateVisibility::Visible);
			UE_LOG(LogTemp, Warning, TEXT("CharacterSelectionPanel a fost adăugat în Viewport!"));
		}
	}
}

void AMainMenuGameMode::RemoveCharacterSelectionPanel()
{
	if (CharacterSelectionPanel)
	{
		CharacterSelectionPanel->RemoveFromParent();
		CharacterSelectionPanel = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("CharacterSelectionPanel a fost ascuns!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Eroare: CharacterSelectionPanel este nullptr!"));
	}
}

void AMainMenuGameMode::ShowCharacterCreationPanel()
{
	if (CharacterCreatePanelClass)
	{
		CharacterCreationPanel = CreateWidget<UUserWidget>(GetWorld(), CharacterCreatePanelClass);
		if (CharacterCreationPanel)
		{
			CharacterCreationPanel->AddToViewport();
			CharacterCreationPanel->SetVisibility(ESlateVisibility::Visible);
			UE_LOG(LogTemp, Warning, TEXT("CharacterCreationPanel a fost adăugat în Viewport!"));
		}
	}
}

void AMainMenuGameMode::RemoveCharacterCreationPanel()
{
	if (CharacterCreationPanel)
	{
		CharacterCreationPanel->RemoveFromParent();
		CharacterCreationPanel = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("CharacterCreationPanel a fost ascuns!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Eroare: CharacterCreationPanel este nullptr!"));
	}
}
