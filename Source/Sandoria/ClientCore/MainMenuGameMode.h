// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Blueprint/UserWidget.h"
#include "Sandoria/UserInterface/MainMapUI/LoginPanel.h"
#include "Sandoria/UserInterface/MainMapUI/CharacterSelectionWidget.h"
#include "Sandoria/UserInterface/MainMapUI/CreateCharacterUI.h"
#include "MainMenuGameMode.generated.h"


UCLASS()
class SANDORIA_API AMainMenuGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMainMenuGameMode();

protected:
    virtual void BeginPlay() override;

public:
    /** Referință la LoginPanel */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> LoginPanelClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> CharacterSelectionPanelClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> CharacterCreatePanelClass;

    UPROPERTY()
    UUserWidget* LoginPanel;

    UPROPERTY()
    UUserWidget* CharacterSelectionPanel;

    UPROPERTY()
    UUserWidget* CharacterCreationPanel;

    void ShowLoginPanel();
    void RemoveLoginPanel();
    
    void ShowCharacterSelectionWidget();
    void RemoveCharacterSelectionPanel();

    void ShowCharacterCreationPanel();
    void RemoveCharacterCreationPanel();
};
