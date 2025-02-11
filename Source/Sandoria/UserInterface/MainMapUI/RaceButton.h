// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Sandoria/StructsAndEnums/GameTypes.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "RaceButton.generated.h"

/**
 * 
 */
UCLASS()
class SANDORIA_API URaceButton : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UButton* SelectRaceButton;

	UPROPERTY(meta = (BindWidget))
	class UBorder* RaceLogoBorder;

	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	ECharacterRace CharacterRace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	ECharacterFaction CharacterFaction;

	ECharacterRace SelectedRace;
	ECharacterFaction SelectedFaction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TMap<ECharacterRace, UTexture2D*> RaceIcons;

	bool IsSelected;

	void SetupRaceButton();

	static TArray<URaceButton*> AllRaceAndFactionButtons; // Stocăm toate butoanele

	UFUNCTION()
	void HandleOnClicked();

	void SetSelected(bool isSelected);
	void DeselectOtherButtons();
};
