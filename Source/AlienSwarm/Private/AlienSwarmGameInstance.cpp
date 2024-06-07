// Fill out your copyright notice in the Description page of Project Settings.


#include "AlienSwarmGameInstance.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"

void UAlienSwarmGameInstance::Init()
{
	Super::Init();

	auto subsys = IOnlineSubsystem::Get();
	if (subsys) {
		sessionInterface = subsys->GetSessionInterface();

		sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UAlienSwarmGameInstance::OnCreateSessionComplete);
		sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UAlienSwarmGameInstance::OnFindSessionComplete);

	}
}

void UAlienSwarmGameInstance::CreateMySession(FString roomName)
{
	FOnlineSessionSettings set;

	set.bIsDedicated = false;
	FName subsysName = IOnlineSubsystem::Get()->GetSubsystemName();
	set.bIsLANMatch = subsysName == "NULL";
	set.bShouldAdvertise = true;
	set.bUsesPresence = true;
	set.bAllowJoinInProgress = true;
	set.bAllowJoinViaPresence = false;
	set.NumPublicConnections = maxPlayerPerRoom;

	set.Set(FName("ROOM_NAME"), StringBase64Encode(roomName), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	set.Set(FName("HOST_NAME"), StringBase64Encode(mySessionName), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();

	sessionInterface->CreateSession(*netID, FName(*roomName), set);
}

void UAlienSwarmGameInstance::OnCreateSessionComplete(FName sessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete -> sessionName : %s, bWasSuccessful : %d"), *sessionName.ToString(), bWasSuccessful);

	// 만약 방생성을 성공했다면 ServerTravel 하고싶다.
	if (bWasSuccessful)
	{
		GetWorld()->ServerTravel(TEXT("/Game/Levels/WaitLevel?listen"));
	}
}

void UAlienSwarmGameInstance::OnFindSessionComplete(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		auto result = sessionInSearch->SearchResults;
		for (auto item : result) {
			if (item.IsValid() == false){
				continue;
			}

			FSessionInfo info;
			info.Set(item);

			FString roomName;
			item.Session.SessionSettings.Get(FName("ROOM_NAME"), roomName);
			FString hostName;
			item.Session.SessionSettings.Get(FName("HOST_NAME"), hostName);

			info.roomName = StringBase64Decode(roomName);
			info.hostName = StringBase64Decode(hostName);

			//OnMySessionSearchCompleteDelegate.Broadcast(info);
		}
	}
}

void UAlienSwarmGameInstance::FindOtherSessions()
{
	sessionInSearch = MakeShareable(new FOnlineSessionSearch);

	sessionInSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	sessionInSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";

	sessionInSearch->MaxSearchResults = 10;

	sessionInterface->FindSessions(0, sessionInSearch.ToSharedRef());
}

FString UAlienSwarmGameInstance::StringBase64Encode(const FString& str)
{
	// Set 할 때 : FString -> UTF8 (std::string) -> TArray<uint8> -> base64 로 Encode
	std::string utf8String = TCHAR_TO_UTF8(*str);
	TArray<uint8> arrayData = TArray<uint8>((uint8*)(utf8String.c_str()), utf8String.length());
	return FBase64::Encode(arrayData);
}

FString UAlienSwarmGameInstance::StringBase64Decode(const FString& str)
{
	// Get 할 때 : base64 로 Decode -> TArray<uint8> -> TCHAR
	TArray<uint8> arrayData;
	FBase64::Decode(str, arrayData);
	std::string ut8String((char*)(arrayData.GetData()), arrayData.Num());
	return UTF8_TO_TCHAR(ut8String.c_str());
}
