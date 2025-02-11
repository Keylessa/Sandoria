// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Sandoria/StructsAndEnums/GameTypes.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "GenderButtonUI.generated.h"

/**
 * 
 */
UCLASS()
class SANDORIA_API UGenderButtonUI : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;
		

	UPROPERTY(meta = (BindWidget))
	UButton* SelectGenderButton;

	UPROPERTY(meta = (BindWidget))
	class UBorder* GenderLogoBorder;



public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	ECharacterGender CharacterGender;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	ECharacterGender SelectedGender;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TMap<ECharacterGender, UTexture2D*> GenderIcons;


	bool IsSelected;

	UFUNCTION()
	void HandleOnClicked();

	void SetupGenderButton();

	static TArray<UGenderButtonUI*> AllGenderButtons; // Stocăm toate butoanele

	void SetSelected(bool isSelected);
	void DeselectOtherButtons();
};
