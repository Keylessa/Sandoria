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
    ECharacterFaction CharacterFaction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    ECharacterRace CharacterRace;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    ECharacterClass CharacterClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    ECharacterGender CharacterGender;
public:
    UPROPERTY()
    uint32 PlayerID;

    UPROPERTY()
    uint32 CharacterID;

    UPROPERTY()
    float PosX;

    UPROPERTY()
    float PosY;

    UPROPERTY()
    float PosZ;


    // Constructor default
    FCharacterStats()
        : Name("TestName"), CharacterLevel(7), CharacterFaction(ECharacterFaction::FACTION_NONE), CharacterRace(ECharacterRace::RACE_NONE), CharacterClass(ECharacterClass::CLASS_NONE), CharacterGender(ECharacterGender::GENDER_MALE)
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

USTRUCT()
struct FEnterWorldPacket
{
    GENERATED_BODY()

    uint32 PlayerID;
    uint32 CharacterID;
    float PosX, PosY, PosZ;

    // Constructor implicit
    FEnterWorldPacket() : PlayerID(0), CharacterID(0), PosX(0), PosY(0), PosZ(0) {}

    // Funcție pentru serializare
    template <typename Archive>
    void Serialize(Archive& Ar)
    {
        // Utilizăm funcția de serializare corectă pentru fiecare tip de date
        Ar << PlayerID;
        Ar << CharacterID;
        Ar << PosX;
        Ar << PosY;
        Ar << PosZ;
    }
};