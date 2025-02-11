// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ActorCharacterPreviewActor.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"

// Sets default values
AActorCharacterPreviewActor::AActorCharacterPreviewActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	RootComponent = CharacterMesh;

	// Adăugăm SceneCaptureComponent2D
	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
	SceneCapture->SetupAttachment(RootComponent);
	SceneCapture->bCaptureEveryFrame = true;
	SceneCapture->bCaptureOnMovement = true;

}

// Called when the game starts or when spawned
void AActorCharacterPreviewActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActorCharacterPreviewActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AActorCharacterPreviewActor::SetCharacterMesh(USkeletalMesh* NewMesh)
{
	if (CharacterMesh && NewMesh)
	{
		CharacterMesh->SetSkeletalMesh(NewMesh);
	}
}

void AActorCharacterPreviewActor::RotateCharacter(float RotationAmount)
{
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += RotationAmount;
	SetActorRotation(NewRotation);
}

