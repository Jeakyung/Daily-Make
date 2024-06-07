// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "AlienSwarmGameInstance.generated.h"

/**
 * 
 */
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

	void CreateMySession(FString roomName);

	UFUNCTION()
	void OnCreateSessionComplete(FName sessionName, bool bWasSuccessful);

	FString mySessionName = TEXT("DailyMake");

	FString StringBase64Encode(const FString& str);
	FString StringBase64Decode(const FString& str);
};
