#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"  // Acest include este necesar!
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Sandoria/ClientCore/SandoriaGameInstance.h"
#include "Sandoria/UserInterface/MainMapUI/CharacterSelectionWidget.h"
#include "LoginPanel.generated.h"

class AMainMenuGameMode;

UCLASS()
class SANDORIA_API ULoginPanel : public UUserWidget
{
	GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;  // Asigură-te că este în secțiunea protected
    virtual void NativeDestruct() override;
private:
    FTimerHandle VisibilityTimerHandle; // Handle pentru timer

public:
    UPROPERTY(meta = (BindWidget))
    class UEditableTextBox* UsernameBox;

    UPROPERTY(meta = (BindWidget))
    class UEditableTextBox* PasswordBox;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* MessageText;

    UPROPERTY(meta = (BindWidget))
    class UButton* LoginButton;


    AMainMenuGameMode* MainMenuGameMode;


    // Referință la widget-ul de selectionare a caracterului
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    UCharacterSelectionWidget* CharacterSelectionWidget;

    // Referință la LoginPanel
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    ULoginPanel* LoginPanel;

    UFUNCTION()
    void OnLoginClicked();
    void HandleLogin();


	
};
