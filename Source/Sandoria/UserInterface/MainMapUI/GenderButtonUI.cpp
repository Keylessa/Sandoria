// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/MainMapUI/GenderButtonUI.h"

void UGenderButtonUI::NativeConstruct()
{
    Super::NativeConstruct();

    SelectedGender = CharacterGender;
    IsSelected = false;

    AllGenderButtons.Add(this); // Adăugăm acest buton în listă
    SetupGenderButton();

    if (SelectGenderButton)
    {
        UE_LOG(LogTemp, Warning, TEXT("SelectGenderButton este valid pentru clasa %d!"), static_cast<int32>(SelectedGender));

        SelectGenderButton->OnClicked.AddDynamic(this, &UGenderButtonUI::HandleOnClicked);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("SelectGenderButton este nullptr!"));
    }
}

TArray<UGenderButtonUI*> UGenderButtonUI::AllGenderButtons = {};

void UGenderButtonUI::SetupGenderButton()
{
    if (GenderIcons.Contains(SelectedGender))
    {
        UTexture2D* SelectedIcon = *GenderIcons.Find(SelectedGender);
        if (SelectedIcon && SelectGenderButton)
        {
            FSlateBrush NewBrush;
            NewBrush.SetResourceObject(SelectedIcon);
            NewBrush.ImageSize = FVector2D(100, 100); // Setează dimensiunea imaginii

            FButtonStyle NewStyle = SelectGenderButton->WidgetStyle;
            NewStyle.Normal = NewBrush;   // Imaginea normală
            NewStyle.Hovered = NewBrush;  // Imaginea când mouse-ul este deasupra
            NewStyle.Pressed = NewBrush;  // Imaginea când butonul este apăsat

            SelectGenderButton->SetStyle(NewStyle);
        }
    }
}

void UGenderButtonUI::HandleOnClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("Butonul a fost apasat pentru clasa %d"), static_cast<int32>(SelectedGender));

    DeselectOtherButtons(); // Deselectează celelalte butoane
    SetSelected(true); // Selectează acest buton
}

void UGenderButtonUI::SetSelected(bool isSelected)
{
    IsSelected = isSelected;

    if (!SelectGenderButton)
    {
        UE_LOG(LogTemp, Error, TEXT("SelectClassButton este nullptr!"));
        return;
    }

    if (GenderIcons.Contains(SelectedGender))
    {
        UTexture2D* SelectedIcon = *GenderIcons.Find(SelectedGender);
        if (SelectedIcon)
        {
            FSlateBrush NewBrush;
            NewBrush.SetResourceObject(SelectedIcon);
            NewBrush.ImageSize = FVector2D(128, 128);

            FButtonStyle NewStyle = SelectGenderButton->WidgetStyle;
            NewStyle.Normal = NewBrush;
            NewStyle.Hovered = NewBrush;
            NewStyle.Pressed = NewBrush;

            SelectGenderButton->SetStyle(NewStyle);
        }
    }

    // Schimbă culoarea borderului dacă există
    if (GenderLogoBorder)
    {
        FLinearColor BorderColor = isSelected ? FLinearColor(0.0f, 1.0f, 0.0f, 1.0f) : FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
        GenderLogoBorder->SetBrushColor(BorderColor);
    }
}

void UGenderButtonUI::DeselectOtherButtons()
{
    UE_LOG(LogTemp, Warning, TEXT("DeselectOtherButtons() a fost apelată."));

    for (UGenderButtonUI* Button : AllGenderButtons)
    {
        if (Button)
        {
            UE_LOG(LogTemp, Warning, TEXT("Verific buton pentru clasa %d"), static_cast<int32>(Button->SelectedGender));

            if (Button != this)
            {
                Button->SetSelected(false);
                UE_LOG(LogTemp, Warning, TEXT("Clasa %d a fost deselectata"), static_cast<int32>(Button->SelectedGender));
            }
        }
    }
}