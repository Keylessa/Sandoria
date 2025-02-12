// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Sockets.h"
#include "Sandoria/StructsAndEnums/GameTypes.h"
#include "Sandoria/ClientCore/Game_GameMode.h"
#include "Kismet/GameplayStatics.h"  
#include "Networking.h"
#include <iostream>
#include <string>
#include "NetworkManager.generated.h"

/**
 * 
 */
UCLASS()
class SANDORIA_API UNetworkManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// Conectarea la server
	bool ConnectToServer(const FString& IP, int32 Port);

	// Deconectarea de la server
	void DisconnectFromServer();

	// Verifică dacă socket-ul este valid
	bool IsSocketValid() const;

	// Această funcție primește un mesaj complet primit de la server
	UFUNCTION()
	void ProcessReceivedMessage(const FString& Message);

	// Trimiterea datelor către server
	bool SendData(const FString& Data);

	// Functie care trimite un EnterWorldPacket
	bool SendData(const FEnterWorldPacket& Packet);

	// Recepționarea datelor de la server (varianta simplificată)
	FString ReceiveData();

	// Recepționarea mesajelor (varianta pe linii)
	FString ReceiveMessage();

	// (Opțional) Tratarea erorilor de rețea – poți expune delegate-uri sau callback-uri
	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);

	void HandleEnterWorldPacket(FEnterWorldPacket packet);

protected:
	// Socket-ul folosit pentru client
	FSocket* ClientSocket = nullptr;
};
