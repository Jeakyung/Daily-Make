// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomListItemUI.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "AlienSwarmGameInstance.h"

void URoomListItemUI::NativeConstruct()
{
	Btn_Join->OnClicked.AddDynamic(this, &URoomListItemUI::OnMyClickJoinRoom);
}

void URoomListItemUI::OnMyClickJoinRoom()
{
	
}

void URoomListItemUI::Setup(const FSessionInfo& info)
{
	Text_RoomName->SetText(FText::FromString(info.roomName));
	int32 currentNum = info.currentPlayerCount;
	Text_Num->SetText(FText::FromString(FString::Printf(TEXT("%d"), currentNum)));
}
