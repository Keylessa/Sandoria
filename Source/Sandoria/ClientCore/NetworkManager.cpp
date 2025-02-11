// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientCore/NetworkManager.h"
#include "SocketSubsystem.h"
#include "Engine/Engine.h"

bool UNetworkManager::ConnectToServer(const FString& IP, int32 Port)
{
	ISocketSubsystem* SocketSubSystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	ClientSocket = SocketSubSystem->CreateSocket(NAME_Stream, TEXT("ClientSocket"), false);

	FIPv4Address ServerAddress;
	if (!FIPv4Address::Parse(IP, ServerAddress))
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectToServer: IP-ul nu poate fi interpretat: %s"), *IP);
		return false;
	}

	TSharedRef<FInternetAddr> Addr = SocketSubSystem->CreateInternetAddr();
	Addr->SetIp(ServerAddress.Value);
	Addr->SetPort(Port);

	bool bConnected = ClientSocket->Connect(*Addr);
	if (bConnected)
	{
		UE_LOG(LogTemp, Warning, TEXT("Conectat la server: %s:%d"), *IP, Port);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Conectarea la server a eșuat: %s:%d"), *IP, Port);
	}

	return bConnected;
}

void UNetworkManager::DisconnectFromServer()
{
	if (ClientSocket)
	{
		ClientSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ClientSocket);
		ClientSocket = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("Deconectat de la server."));
	}
}

bool UNetworkManager::IsSocketValid() const
{
	return ClientSocket && ClientSocket->GetConnectionState() == SCS_Connected;
}

bool UNetworkManager::SendData(const FString& Data)
{
	if (!IsSocketValid())
	{
		UE_LOG(LogTemp, Error, TEXT("SendData: Socket-ul nu este valid!"));
		return false;
	}

	FTCHARToUTF8 Converted(*Data);
	int32 Size = Converted.Length();
	int32 Sent = 0;

	bool bSuccess = ClientSocket->Send((uint8*)Converted.Get(), Size, Sent);
	if (!bSuccess)
	{
		UE_LOG(LogTemp, Error, TEXT("SendData: Eroare la trimiterea datelor către server!"));
	}
	return bSuccess;
}

FString UNetworkManager::ReceiveData()
{
	if (!IsSocketValid())
	{
		UE_LOG(LogTemp, Error, TEXT("ReceiveData: Socket-ul nu este valid!"));
		return "";
	}

	uint8 Data[1024] = { 0 };
	int32 BytesRead = 0;

	if (ClientSocket->Recv(Data, sizeof(Data), BytesRead))
	{
		FString Result = FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(Data))).TrimStartAndEnd();
		UE_LOG(LogTemp, Warning, TEXT("Mesaj primit de la server: %s"), *Result);
		return Result;
	}

	UE_LOG(LogTemp, Error, TEXT("ReceiveData: Nu s-au putut primi date de la server!"));
	return "";
}

FString UNetworkManager::ReceiveMessage()
{
	if (!IsSocketValid())
	{
		UE_LOG(LogTemp, Error, TEXT("ReceiveMessage: Socket-ul nu este valid!"));
		return "";
	}

	FString Accumulated;
	uint8 Buffer[256] = { 0 };
	int32 BytesRead = 0;

	// Citim în buclă până găsim separatorul de linie "\n"
	while (ClientSocket->Recv(Buffer, sizeof(Buffer), BytesRead))
	{
		FString Part = FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(Buffer)));
		Accumulated += Part;
		if (Accumulated.Contains("\n"))
		{
			break;
		}
	}

	TArray<FString> Messages;
	Accumulated.ParseIntoArray(Messages, TEXT("\n"), true);
	if (Messages.Num() > 0)
	{
		FString FirstMessage = Messages[0].TrimStartAndEnd();
		UE_LOG(LogTemp, Warning, TEXT("Received complete message: %s"), *FirstMessage);
		return FirstMessage;
	}

	return "";
}

void UNetworkManager::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	UE_LOG(LogTemp, Error, TEXT("Network failure: %s"), *ErrorString);
	// Aici poți adăuga logica pentru reconectare sau notificare
}