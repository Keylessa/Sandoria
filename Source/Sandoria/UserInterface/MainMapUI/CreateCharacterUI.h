// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Sandoria/UserInterface/MainMapUI/RaceButton.h"
#include "Sandoria/UserInterface/MainMapUI/ClassButtonUI.h"
#include "Sandoria/UserInterface/MainMapUI/GenderButtonUI.h"
#include "CreateCharacterUI.generated.h"



/**
 * 
 */
UCLASS()
class SANDORIA_API UCreateCharacterUI : public UUserWidget
{
	GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

public:
    // Butoane
    UPROPERTY(meta = (BindWidget))
    class UButton* Button_Customize;

    UPROPERTY(meta = (BindWidget))
    class UButton* Button_Back_Customize;

    UPROPERTY(meta = (BindWidget))
    class UButton* Button_Finish;

    UPROPERTY(meta = (BindWidget))
    class UButton* BackToCharacterSelectionButton;

    UPROPERTY(meta = (BindWidget))
    UEditableTextBox* NameTextBox;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ErrorMessageText;

    // Panouri UI
    UPROPERTY(meta = (BindWidget))
    class UCanvasPanel* MainPanel;

    UPROPERTY(meta = (BindWidget))
    class UCanvasPanel* CustomizePanel;

    URaceButton* RaceAndFactionButton;
    UClassButtonUI* ClassButton;
    UGenderButtonUI* GenderButton;

	template <typename T>
	T* GetSelectedButton(TArray<T*>& ButtonList)
	{
		for (T* Button : ButtonList)
		{
			if (Button && Button->IsSelected)
			{
				return Button;
			}
		}
		return nullptr;
	}

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Core")
    class USandoriaGameInstance* GameInstances;

    // Funcții pentru gestionarea UI
    UFUNCTION()
    void OnCustomizeClicked();

    UFUNCTION()
    void OnBackCustomizeClicked();

    UFUNCTION()
    void OnBackToCharSelectClicked();

    UFUNCTION()
    void OnFinishClicked();

	FString FactionToString(ECharacterFaction Faction)
	{
		switch (Faction)
		{
		case ECharacterFaction::FACTION_NONE: return "0";

		case ECharacterFaction::FACTION_SOLVARION: return "1";

		case ECharacterFaction::FACTION_NOCTARION: return "2";

		default:return "0";
		}
	}

	FString RaceToString(ECharacterRace Race)
	{
		switch (Race)
		{
		case ECharacterRace::RACE_NONE: return "0";

		case ECharacterRace::RACE_ELF: return "1";

		case ECharacterRace::RACE_VOID_ELF: return "2";

		default: return "0";
		}
	}

	FString ClassToString(ECharacterClass Class)
	{
		switch (Class)
		{
		case ECharacterClass::CLASS_NONE: return "0";

		case ECharacterClass::CLASS_WARRIOR: return "1";

		case ECharacterClass::CLASS_PALADIN: return "2";

		case ECharacterClass::CLASS_HUNTER: return "3";

		case ECharacterClass::CLASS_ROGUE: return "4";

		case ECharacterClass::CLASS_PRIEST: return "5";

		case ECharacterClass::CLASS_DEATH_KNIGHT: return "6";

		case ECharacterClass::CLASS_SHAMAN: return "7";

		case ECharacterClass::CLASS_MAGE: return "8";

		case ECharacterClass::CLASS_WARLOCK: return "9";

		case ECharacterClass::CLASS_DRUID: return "10";

		default: return "0";
		}
	}

	FString GenderToString(ECharacterGender Gender)
	{
		switch (Gender)
		{
		case ECharacterGender::GENDER_MALE: return "1";

		case ECharacterGender::GENDER_FEMALE: return "2";

		default: return "0";
		}
	}

	
};
