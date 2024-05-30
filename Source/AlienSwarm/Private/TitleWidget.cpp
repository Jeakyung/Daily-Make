// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Animation/WidgetAnimation.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void UTitleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_Create->OnClicked.AddDynamic(this, &UTitleWidget::CreateRoom);

	Btn_Join->OnClicked.AddDynamic(this, &UTitleWidget::ShowRoomList);

	Btn_Quit->OnClicked.AddDynamic(this, &UTitleWidget::QuitGame);

	Btn_CloseRoomList->OnClicked.AddDynamic(this, &UTitleWidget::CloseRoomList);
}

void UTitleWidget::CreateRoom()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("TestLevel"));
}

void UTitleWidget::ShowRoomList()
{
	PlayAnimation(RoomListSpan);
}

void UTitleWidget::CloseRoomList()
{
	float animPlayTime = RoomListSpan->GetEndTime();
	PlayAnimation(RoomListSpan, 0.0f, 1, EUMGSequencePlayMode::Reverse);
}

void UTitleWidget::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}
