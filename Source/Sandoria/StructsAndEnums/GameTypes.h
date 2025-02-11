#pragma once

#include "CoreMinimal.h"
#include "GameTypes.generated.h"

// Enum care este accesibil și în Blueprint
UENUM(BlueprintType)
enum class ECharacterFaction : uint8
{
    FACTION_NONE UMETA(DisplayName = "None"),
    FACTION_SOLVARION UMETA(DisplayName = "Solvarion"),
    FACTION_NOCTARION UMETA(DisplayName = "Noctarion")
};

UENUM(BlueprintType)
enum class ECharacterRace : uint8
{
    RACE_NONE UMETA(DisplayName = "None"),
    RACE_ELF UMETA(DisplayName = "Elf"),
    RACE_VOID_ELF UMETA(DisplayName = "Void Elf")
};

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
    CLASS_NONE UMETA(DisplayName = "None"),
    CLASS_WARRIOR UMETA(DisplayName = "Warrior"),
    CLASS_PALADIN UMETA(DisplayName = "Paladin"),
    CLASS_HUNTER UMETA(DisplayName = "Hunter"),
    CLASS_ROGUE UMETA(DisplayName = "Rogue"),
    CLASS_PRIEST UMETA(DisplayName = "Priest"),
    CLASS_DEATH_KNIGHT UMETA(DisplayName = "Death Knight"),
    CLASS_SHAMAN UMETA(DisplayName = "Shaman"),
    CLASS_MAGE UMETA(DisplayName = "Mage"),
    CLASS_WARLOCK UMETA(DisplayName = "Warlock"),
    CLASS_DRUID UMETA(DisplayName = "Druid")
};



UENUM(BlueprintType)
enum class ECharacterGender : uint8
{
    GENDER_MALE UMETA(DisplayName = "Male"),
    GENDER_FEMALE UMETA(DisplayName = "Female")
};



USTRUCT(BlueprintType)
struct FCharacterStats
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 CharacterLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 MaxHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 CurHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 MaxMana;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 CurMana;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    ECharacterFaction CharacterFaction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    ECharacterRace CharacterRace;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    ECharacterClass CharacterClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    ECharacterGender CharacterGender;
    // Constructor default
    FCharacterStats()
        : Name("TestName"), CharacterLevel(7), MaxHealth(100), CurHealth(100), MaxMana(50), CurMana(50), CharacterFaction(ECharacterFaction::FACTION_NONE), CharacterRace(ECharacterRace::RACE_NONE), CharacterClass(ECharacterClass::CLASS_NONE)
    {
    }
};


USTRUCT(BlueprintType)
struct FClassIcons
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    UTexture2D* Icon;

};