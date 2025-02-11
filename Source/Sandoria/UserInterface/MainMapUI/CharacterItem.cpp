#include "CharacterItem.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Border.h"

void UCharacterItem::NativeConstruct()
{
    Super::NativeConstruct();

    IsSelected = false;

    // Inițializează structura cu valorile default
    CharacterStats = FCharacterStats();

    if (SelectCharacterButton)
    {
        SelectCharacterButton->OnClicked.AddDynamic(this, &UCharacterItem::HandleOnClicked);
    }
}

void UCharacterItem::SetupCharacter(const FCharacterStats& NewCharacterStats)
{
    CharacterStats = NewCharacterStats;

    // Setează textul în UI
    if (CharacterNameText) CharacterNameText->SetText(FText::FromString(CharacterStats.Name));
    if (CharacterLevelText) CharacterLevelText->SetText(FText::AsNumber(CharacterStats.CharacterLevel));
}

void UCharacterItem::HandleOnClicked()
{
    if (OnCharacterSelected.IsBound())
    {
        OnCharacterSelected.Execute(CharacterIndex);
    }
}

void UCharacterItem::SetSelected(bool isSelected)
{
    IsSelected = isSelected;

    if (ClassLogoBorder)
    {
        ClassLogoBorder->SetBrushColor(isSelected ? FLinearColor(0.0f, 1.0f, 0.0f, 1.0f) : FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
    }
}

// Getteri
FString UCharacterItem::GetCharacterName() const { return CharacterStats.Name; }
int32 UCharacterItem::GetCharacterLevel() const { return CharacterStats.CharacterLevel; }

// Setteri
void UCharacterItem::SetCharacterName(const FString& NewName) 
{ 
    CharacterStats.Name = NewName;
    if (CharacterNameText)
    {
        CharacterNameText->SetText(FText::FromString(NewName));
    }
}
void UCharacterItem::SetCharacterLevel(int32 NewLevel) 
{
    CharacterStats.CharacterLevel = NewLevel;
    if (CharacterLevelText)
    {
        CharacterLevelText->SetText(FText::AsNumber(NewLevel));
    }
}

