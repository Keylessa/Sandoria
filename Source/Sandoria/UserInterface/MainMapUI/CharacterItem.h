#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Sandoria/StructsAndEnums/GameTypes.h" // Include fișierul unde ai definit `FCharacterStats`
#include "CharacterItem.generated.h"

DECLARE_DELEGATE_OneParam(FOnCharacterSelected, int32);

UCLASS()
class SANDORIA_API UCharacterItem : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    void SetupCharacter(const FCharacterStats& NewCharacterStats);
   
    UFUNCTION()
    void HandleOnClicked();

    void SetSelected(bool isSelected);

    // Getteri pentru informații despre caracter
    FString GetCharacterName() const;
    int32 GetCharacterLevel() const;

    // Setteri pentru actualizarea caracterului
    void SetCharacterName(const FString& NewName);
    void SetCharacterLevel(int32 NewLevel);

    // Delegate pentru selectarea caracterului
    FOnCharacterSelected OnCharacterSelected;

    int32 CharacterIndex;

protected:
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* CharacterNameText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* CharacterLevelText;

    UPROPERTY(meta = (BindWidget))
    class UButton* SelectCharacterButton;

    UPROPERTY(meta = (BindWidget))
    class UBorder* ClassLogoBorder;

    


private:
    FCharacterStats CharacterStats;
    //int32 CharacterIndex;
    bool IsSelected;
};
