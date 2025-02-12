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

void UNetworkManager::ProcessReceivedMessage(const FString& Message)
{
	// Dacă mesajul începe cu "ENTER_WORLD:"
	if (Message.StartsWith("ENTER_WORLD:"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Raw server message: %s"), *Message);

		// Eliminăm prefixul "ENTER_WORLD:" (12 caractere)
		FString Data = Message.RightChop(12).TrimStartAndEnd();
		UE_LOG(LogTemp, Warning, TEXT("Processed ENTER_WORLD data: %s"), *Data);

		// Se presupune că datele sunt separate prin virgulă: PlayerID,CharacterID,PosX,PosY,PosZ
		TArray<FString> Tokens;
		Data.ParseIntoArray(Tokens, TEXT(","), true);

		if (Tokens.Num() >= 5)
		{
			int32 PlayerID = FCString::Atoi(*Tokens[0]);
			int32 CharacterID = FCString::Atoi(*Tokens[1]);
			FVector SpawnLocation(
				FCString::Atof(*Tokens[2]),
				FCString::Atof(*Tokens[3]),
				FCString::Atof(*Tokens[4])
			);

			UE_LOG(LogTemp, Warning, TEXT("Spawning PlayerID %d at %s"), PlayerID, *SpawnLocation.ToString());

			AGame_GameMode* GM = Cast<AGame_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
			if (GM)
			{
				GM->SpawnPlayerInWorld(PlayerID, CharacterID, SpawnLocation);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("ProcessReceivedMessage: GameMode is null!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Invalid ENTER_WORLD message format: %s"), *Data);
		}
	}
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

bool UNetworkManager::SendData(const FEnterWorldPacket& Packet)
{
	FBufferArchive Archive;

	// Creăm o copie non-const pentru serializare
	FEnterWorldPacket TempPacket = Packet;
	TempPacket.Serialize(Archive);

	// Convertim buffer-ul într-un TArray<uint8>
	TArray<uint8> Data = Archive;

	// Convertim array-ul într-un FString
	FString SerializedData = FString(UTF8_TO_TCHAR(reinterpret_cast<const char*>(Data.GetData())));

	return SendData(SerializedData);
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

void UNetworkManager::HandleEnterWorldPacket(FEnterWorldPacket packet)
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (AGame_GameMode* GM = Cast<AGame_GameMode>(UGameplayStatics::GetGameMode(this)))
		{
			if (GM) {
				// Creează un FVector din valorile poziției din pachet
				FVector SpawnLocation(packet.PosX, packet.PosY, packet.PosZ);
				// Apelează funcția cu argumentele separate
				GM->SpawnPlayerInWorld(packet.PlayerID, packet.CharacterID, SpawnLocation);
			}
			else 
			{
				UE_LOG(LogTemp, Error, TEXT("GameMode is not valid!"));
			}
		}
		else 
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to get GameMode."));
		}
	}
}
