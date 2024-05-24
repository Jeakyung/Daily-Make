// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBoxSlot.h"

void UTitleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_Create->OnClicked.AddDynamic(this, &UTitleWidget::CreateRoom);

	Btn_Join->OnClicked.AddDynamic(this, &UTitleWidget::ShowRoomList);

	Btn_Quit->OnClicked.AddDynamic(this, &UTitleWidget::QuitGame);

	RoomList->Set
}

void UTitleWidget::CreateRoom()
{
	
}

void UTitleWidget::ShowRoomList()
{
	
}

void UTitleWidget::QuitGame()
{
	
}
