// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "AlienSwarmGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FSessionInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString roomName;
	UPROPERTY(BlueprintReadOnly)
	FString hostName;
	UPROPERTY(BlueprintReadOnly)
	FString userName;
	UPROPERTY(BlueprintReadOnly)
	int32 currentPlayerCount;

	FORCEINLINE void Set(const FOnlineSessionSearchResult& item) {
		//item.Session.SessionSettings.Get(FName("ROOM_NAME"), roomName);
		//item.Session.SessionSettings.Get(FName("HOST_NAME"), hostName);
		// ������ �̸�
		userName = item.Session.OwningUserName;
		// ���� �濡 ���� �÷��̾� ��
		currentPlayerCount = 4 - item.Session.NumOpenPublicConnections;
	}

};

// ��ã�� ��û �� ������ ���� �� ȣ��� ��������Ʈ
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSessioinSearchDelegate, const FSessionInfo&, info);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSessioinSearchFinishedDelegate, bool, bSearching);

UCLASS()
class ALIENSWARM_API UAlienSwarmGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;

	UPROPERTY(EditDefaultsOnly)
	int32 maxPlayerPerRoom = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 totalCurrency = 20000;

	IOnlineSessionPtr sessionInterface;

	FString mySessionName = TEXT("DailyMake");

	TSharedPtr<FOnlineSessionSearch> sessionInSearch;

	void CreateMySession(FString roomName);

	UFUNCTION()
	void OnCreateSessionComplete(FName sessionName, bool bWasSuccessful);

	UFUNCTION()
	void OnFindSessionComplete(bool bWasSuccessful);

	void FindOtherSessions();

	FString StringBase64Encode(const FString& str);
	FString StringBase64Decode(const FString& str);
};
