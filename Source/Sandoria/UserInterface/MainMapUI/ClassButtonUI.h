// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Sandoria/StructsAndEnums/GameTypes.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "ClassButtonUI.generated.h"

/**
 * 
 */
UCLASS()
class SANDORIA_API UClassButtonUI : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleOnClicked();

	UPROPERTY(meta = (BindWidget))
	UButton* SelectClassButton;

	UPROPERTY(meta = (BindWidget))
	class UBorder* ClassLogoBorder;
	
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	ECharacterClass CharacterClass;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	ECharacterClass SelectedClass;
	

	bool IsSelected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TMap<ECharacterClass, UTexture2D*> ClassIcons;
	
	void SetupClassButton();

	static TArray<UClassButtonUI*> AllClassButtons; // Stocăm toate butoanele
	
	void SetSelected(bool isSelected);
	void DeselectOtherButtons();


};
