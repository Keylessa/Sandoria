// CharacterSelectionWidget.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Sandoria/StructsAndEnums/GameTypes.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "CharacterItem.h"
#include "Sandoria/ClientCore/SandoriaGameInstance.h"  // Include structura FCharacterData
#include "CharacterSelectionWidget.generated.h"

/**
 * Widget pentru selecția caracterelor din contul jucătorului.
 */
UCLASS()
class SANDORIA_API UCharacterSelectionWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	// Butoane UI
	UPROPERTY(meta = (BindWidget))
	UButton* CreateCharacterButton;

	UPROPERTY(meta = (BindWidget))
	UButton* DeleteCharacterButton;

	UPROPERTY(meta = (BindWidget))
	UButton* BackToLoginButton;

	UPROPERTY(meta = (BindWidget))
	UButton* EnterToWorldButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (BindWidget), Category = "UI")
	UVerticalBox* CharacterListBox;

	// Clasa widgetului pentru caractere
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UCharacterItem> CharacterItemClass;

	// Caracterul selectat
	UPROPERTY(BlueprintReadWrite, Category = "Character Selection")
	FCharacterStats SelectedCharacter;

	UPROPERTY()
	AActor* CharacterPreviewActor; // Actorul vizual din UI

	// Funcții pentru butoane
	UFUNCTION()
	void OnCreateCharacterClicked();

	UFUNCTION()
	void OnDeleteCharacterClicked();

	UFUNCTION()
	void OnBackToLoginClicked();

	UFUNCTION()
	void OnEnterToWorldClicked();

	void SelectCharacter(int32 Index); // Funcția care selectează un caracter
	void UpdateCharacterPreview(); // Funcția care actualizează actorul de previzualizare

	// Gestionarea listei de caractere
	void PopulateCharacterList();

private:
	UFUNCTION()
	void OnCharacterSelected(UCharacterItem* SelectedWidget);
};
