// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/MainMapUI/ClassButtonUI.h"

void UClassButtonUI::NativeConstruct()
{
    Super::NativeConstruct();

    SelectedClass = CharacterClass;
    IsSelected = false;

    AllClassButtons.Add(this); // Adăugăm acest buton în listă
    SetupClassButton();

    if (SelectClassButton)
    {
        UE_LOG(LogTemp, Warning, TEXT("SelectClassButton este valid pentru clasa %d!"), static_cast<int32>(SelectedClass));

        SelectClassButton->OnClicked.AddDynamic(this, &UClassButtonUI::HandleOnClicked);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("SelectClassButton este nullptr!"));
    }
}

TArray<UClassButtonUI*> UClassButtonUI::AllClassButtons = {};

void UClassButtonUI::SetupClassButton()
{
    if (ClassIcons.Contains(SelectedClass))
    {
        UTexture2D* SelectedIcon = *ClassIcons.Find(SelectedClass);
        if (SelectedIcon && SelectClassButton)
        {
            FSlateBrush NewBrush;
            NewBrush.SetResourceObject(SelectedIcon);
            NewBrush.ImageSize = FVector2D(100, 100); // Setează dimensiunea imaginii

            FButtonStyle NewStyle = SelectClassButton->WidgetStyle;
            NewStyle.Normal = NewBrush;   // Imaginea normală
            NewStyle.Hovered = NewBrush;  // Imaginea când mouse-ul este deasupra
            NewStyle.Pressed = NewBrush;  // Imaginea când butonul este apăsat

            SelectClassButton->SetStyle(NewStyle);
        }
    }
}

void UClassButtonUI::HandleOnClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("Butonul a fost apasat pentru clasa %d"), static_cast<int32>(SelectedClass));

    DeselectOtherButtons(); // Deselectează celelalte butoane
    SetSelected(true); // Selectează acest buton
}

void UClassButtonUI::SetSelected(bool isSelected)
{
    IsSelected = isSelected;

    if (!SelectClassButton)
    {
        UE_LOG(LogTemp, Error, TEXT("SelectClassButton este nullptr!"));
        return;
    }

    if (ClassIcons.Contains(SelectedClass))
    {
        UTexture2D* SelectedIcon = *ClassIcons.Find(SelectedClass);
        if (SelectedIcon)
        {
            FSlateBrush NewBrush;
            NewBrush.SetResourceObject(SelectedIcon);
            NewBrush.ImageSize = FVector2D(128, 128);

            FButtonStyle NewStyle = SelectClassButton->WidgetStyle;
            NewStyle.Normal = NewBrush;
            NewStyle.Hovered = NewBrush;
            NewStyle.Pressed = NewBrush;

            SelectClassButton->SetStyle(NewStyle);
        }
    }

    // Schimbă culoarea borderului dacă există
    if (ClassLogoBorder)
    {
        FLinearColor BorderColor = isSelected ? FLinearColor(0.0f, 1.0f, 0.0f, 1.0f) : FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
        ClassLogoBorder->SetBrushColor(BorderColor);
    }
}

void UClassButtonUI::DeselectOtherButtons()
{
    UE_LOG(LogTemp, Warning, TEXT("DeselectOtherButtons() a fost apelată."));

    for (UClassButtonUI* Button : AllClassButtons)
    {
        if (Button)
        {
            UE_LOG(LogTemp, Warning, TEXT("Verific buton pentru clasa %d"), static_cast<int32>(Button->SelectedClass));

            if (Button != this)
            {
                Button->SetSelected(false);
                UE_LOG(LogTemp, Warning, TEXT("Clasa %d a fost deselectata"), static_cast<int32>(Button->SelectedClass));
            }
        }
    }
}