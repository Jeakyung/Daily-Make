// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RoomListItemUI.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSWARM_API URoomListItemUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* Btn_Join;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* Text_RoomName;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* Text_Num;

	int32 index;

	UFUNCTION()
	void OnMyClickJoinRoom();

	void Setup(const struct FSessionInfo& info);
};
