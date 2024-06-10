// Fill out your copyright notice in the Description page of Project Settings.

#include "GameOverWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/CanvasPanel.h>
#include "TestPlayerController.h"


void UGameOverWidget::NativeConstruct()
{
	Button_Quit->OnClicked.AddDynamic(this, &UGameOverWidget::OnClickQuit);
	Button_ShowOP->OnClicked.AddDynamic(this, &UGameOverWidget::OnClickShowOP);
}



void UGameOverWidget::OnClickQuit()
{
	auto* pc = GetWorld()->GetFirstPlayerController();
	pc->ClientTravel("/Game/Levels/TitleLevel", ETravelType::TRAVEL_Absolute);
}

void UGameOverWidget::OnClickShowOP()
{
	// 관전 모드 실행
	SetVisibility(ESlateVisibility::Hidden);
}
