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
		sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UAlienSwarmGameInstance::OnJoinSessionComplete);
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

	// ���� ������� �����ߴٸ� ServerTravel �ϰ�ʹ�.
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
		for (int32 i = 0; i < result.Num(); i++) {
			if (result[i].IsValid() == false) {
				continue;
			}

			FSessionInfo info;
			info.Set(i, result[i]);

			FString roomName;
			result[i].Session.SessionSettings.Get(FName("ROOM_NAME"), roomName);
			FString hostName;
			result[i].Session.SessionSettings.Get(FName("HOST_NAME"), hostName);

			info.roomName = StringBase64Decode(roomName);
			info.hostName = StringBase64Decode(hostName);

			OnMySessionSearchCompleteDelegate.Broadcast(info);
		}
	}
	else {
		if (OnMySessionSearchFinisherDelegate.IsBound()) {
			OnMySessionSearchFinisherDelegate.Broadcast(false);
		}
	}
}

void UAlienSwarmGameInstance::FindOtherSessions()
{
	if (OnMySessionSearchFinisherDelegate.IsBound()) {
		OnMySessionSearchFinisherDelegate.Broadcast(true);
	}

	sessionInSearch = MakeShareable(new FOnlineSessionSearch);

	sessionInSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	sessionInSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";

	sessionInSearch->MaxSearchResults = 10;

	sessionInterface->FindSessions(0, sessionInSearch.ToSharedRef());
}

FString UAlienSwarmGameInstance::StringBase64Encode(const FString& str)
{
	// Set �� �� : FString -> UTF8 (std::string) -> TArray<uint8> -> base64 �� Encode
	std::string utf8String = TCHAR_TO_UTF8(*str);
	TArray<uint8> arrayData = TArray<uint8>((uint8*)(utf8String.c_str()), utf8String.length());
	return FBase64::Encode(arrayData);
}

void UAlienSwarmGameInstance::JoinMySession(int32 index)
{
	sessionInterface->JoinSession(0, FName(*mySessionName), sessionInSearch->SearchResults[index]);
}

FString UAlienSwarmGameInstance::StringBase64Decode(const FString& str)
{
	// Get �� �� : base64 �� Decode -> TArray<uint8> -> TCHAR
	TArray<uint8> arrayData;
	FBase64::Decode(str, arrayData);
	std::string ut8String((char*)(arrayData.GetData()), arrayData.Num());
	return UTF8_TO_TCHAR(ut8String.c_str());
}

void UAlienSwarmGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type result)
{
	if (result == EOnJoinSessionCompleteResult::Success) {
		auto* pc = GetWorld()->GetFirstPlayerController();
		FString url;
		sessionInterface->GetResolvedConnectString(SessionName, url);

		if (!url.IsEmpty()) {
			pc->ClientTravel(url, ETravelType::TRAVEL_Absolute);
		}
	}
}
