// Fill out your copyright notice in the Description page of Project Settings.


#include "TestPlayerController.h"
#include "TitleWidget.h"

void ATestPlayerController::BeginPlay()
{
	//현재 열려있는 맵의 이름이 TitleLevel이면(실제 리턴되는 값은 UEDPIE_0_TitleLevel임)
	if (GetWorld()->GetMapName().Contains(TEXT("TitleLevel"))) {
		//타이틀 위젯을 열어주자
		if (titleWidget_bp) {
			titleWidget = CreateWidget<UTitleWidget>(GetWorld(), titleWidget_bp);
			if (titleWidget) {
				titleWidget->AddToViewport();
			}
		}
	}
}
