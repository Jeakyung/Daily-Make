// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Animation/WidgetAnimation.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "AlienSwarmGameInstance.h"
#include "Components/EditableText.h"

void UTitleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	gi = Cast<UAlienSwarmGameInstance>(GetWorld()->GetGameInstance());

	Btn_Create->OnClicked.AddDynamic(this, &UTitleWidget::ShowRoomSetting);

	Btn_CloseRoomSetting->OnClicked.AddDynamic(this, &UTitleWidget::CloseRoomSetting);

	Btn_InsideCreate->OnClicked.AddDynamic(this, &UTitleWidget::CreateRoom);

	Btn_Join->OnClicked.AddDynamic(this, &UTitleWidget::ShowRoomList);

	Btn_CloseRoomList->OnClicked.AddDynamic(this, &UTitleWidget::CloseRoomList);

	Btn_Quit->OnClicked.AddDynamic(this, &UTitleWidget::QuitGame);
}

void UTitleWidget::ShowRoomSetting()
{
	PlayAnimation(RoomSettingSpan);
}

void UTitleWidget::CloseRoomSetting()
{
	PlayAnimation(RoomSettingSpan, 0.0f, 1, EUMGSequencePlayMode::Reverse);
}

void UTitleWidget::CreateRoom()
{
	if (gi) {
		gi->CreateMySession(EditedRoomName->GetText().ToString());
	}
}

void UTitleWidget::ShowRoomList()
{
	PlayAnimation(RoomListSpan);
}

void UTitleWidget::CloseRoomList()
{
	PlayAnimation(RoomListSpan, 0.0f, 1, EUMGSequencePlayMode::Reverse);
}

void UTitleWidget::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);
}
