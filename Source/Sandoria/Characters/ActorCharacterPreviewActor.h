// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorCharacterPreviewActor.generated.h"

UCLASS()
class SANDORIA_API AActorCharacterPreviewActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorCharacterPreviewActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Mesh pentru personaj
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character Preview")
	class USkeletalMeshComponent* CharacterMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character Preview")
	class USceneCaptureComponent2D* SceneCapture;

	// Funcție pentru setarea modelului personajului
	void SetCharacterMesh(USkeletalMesh* NewMesh);

	// Funcție pentru rotirea personajului
	void RotateCharacter(float RotationAmount);

};
