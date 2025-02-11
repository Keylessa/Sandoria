// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/MainMapUI/RaceButton.h"

void URaceButton::NativeConstruct()
{
    Super::NativeConstruct();

    SelectedRace = CharacterRace;
    SelectedFaction = CharacterFaction;
    IsSelected = false;

    AllRaceAndFactionButtons.Add(this); // Adăugăm acest buton în listă
    SetupRaceButton();

    if (SelectRaceButton)
    {
        UE_LOG(LogTemp, Warning, TEXT("SelectRaceButton este valid pentru Rasa %d!"), static_cast<int32>(SelectedRace));
        UE_LOG(LogTemp, Warning, TEXT("SelectRaceButton este valid pentru Factiunea %d!"), static_cast<int32>(SelectedFaction));

        SelectRaceButton->OnClicked.AddDynamic(this, &URaceButton::HandleOnClicked);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("SelectRaceButton este nullptr!"));
    }
}

TArray<URaceButton*> URaceButton::AllRaceAndFactionButtons = {};

void URaceButton::HandleOnClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("Butonul a fost apasat pentru Rasa %d"), static_cast<int32>(SelectedRace));

    DeselectOtherButtons(); // Deselectează celelalte butoane
    SetSelected(true); // Selectează acest buton
}

void URaceButton::SetupRaceButton()
{
    if (RaceIcons.Contains(SelectedRace))
    {
        UTexture2D* SelectedIcon = *RaceIcons.Find(SelectedRace);
        if (SelectedIcon && SelectRaceButton)
        {
            FSlateBrush NewBrush;
            NewBrush.SetResourceObject(SelectedIcon);
            NewBrush.ImageSize = FVector2D(100, 100); // Setează dimensiunea imaginii

            FButtonStyle NewStyle = SelectRaceButton->WidgetStyle;
            NewStyle.Normal = NewBrush;   // Imaginea normală
            //NewStyle.Hovered = NewBrush;  // Imaginea când mouse-ul este deasupra
            NewStyle.Pressed = NewBrush;  // Imaginea când butonul este apăsat

            SelectRaceButton->SetStyle(NewStyle);
        }
    }
}

void URaceButton::SetSelected(bool isSelected)
{
    IsSelected = isSelected;

    if (!SelectRaceButton)
    {
        UE_LOG(LogTemp, Error, TEXT("SelectClassButton este nullptr!"));
        return;
    }

    if (RaceIcons.Contains(SelectedRace))
    {
        UTexture2D* SelectedIcon = *RaceIcons.Find(SelectedRace);
        if (SelectedIcon)
        {
            FSlateBrush NewBrush;
            NewBrush.SetResourceObject(SelectedIcon);
            NewBrush.ImageSize = FVector2D(128, 128);

            FButtonStyle NewStyle = SelectRaceButton->WidgetStyle;
            NewStyle.Normal = NewBrush;
            // NewStyle.Hovered = NewBrush;
            NewStyle.Pressed = NewBrush;

            SelectRaceButton->SetStyle(NewStyle);
        }
    }

    // Schimbă culoarea borderului dacă există
    if (RaceLogoBorder)
    {
        FLinearColor BorderColor = isSelected ? FLinearColor(0.0f, 1.0f, 0.0f, 1.0f) : FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
        RaceLogoBorder->SetBrushColor(BorderColor);
    }
}

void URaceButton::DeselectOtherButtons()
{
    UE_LOG(LogTemp, Warning, TEXT("DeselectOtherButtons() a fost apelată."));

    for (URaceButton* Button : AllRaceAndFactionButtons)
    {
        if (Button)
        {
            UE_LOG(LogTemp, Warning, TEXT("Verific buton pentru Race %d"), static_cast<int32>(Button->SelectedRace));

            if (Button != this)
            {
                Button->SetSelected(false);
                UE_LOG(LogTemp, Warning, TEXT("Race %d a fost deselectata"), static_cast<int32>(Button->SelectedRace));
            }
        }
    }
}
