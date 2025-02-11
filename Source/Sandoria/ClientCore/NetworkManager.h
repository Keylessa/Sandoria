// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Sockets.h"
#include "Networking.h"
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

	// Trimiterea datelor către server
	bool SendData(const FString& Data);

	// Recepționarea datelor de la server (varianta simplificată)
	FString ReceiveData();

	// Recepționarea mesajelor (varianta pe linii)
	FString ReceiveMessage();

	// (Opțional) Tratarea erorilor de rețea – poți expune delegate-uri sau callback-uri
	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);

protected:
	// Socket-ul folosit pentru client
	FSocket* ClientSocket = nullptr;
};
