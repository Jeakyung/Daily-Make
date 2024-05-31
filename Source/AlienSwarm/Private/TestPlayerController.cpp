// Fill out your copyright notice in the Description page of Project Settings.


#include "TestPlayerController.h"
#include "TitleWidget.h"
#include "ShopWidget.h"
#include "MainWidget.h"

void ATestPlayerController::BeginPlay()
{
	
}

void ATestPlayerController::MakeTitleWidget()
{
	//타이틀 위젯을 열어주자
	if (titleWidget_bp) {
		titleWidget = CreateWidget<UTitleWidget>(GetWorld(), titleWidget_bp);
		if (titleWidget) {
			titleWidget->AddToViewport();
		}
	}
}

void ATestPlayerController::MakeShopWidget()
{
	//샵 위젯을 열어주자.
	if (shopWidget_bp) {
		shopWidget = CreateWidget<UShopWidget>(GetWorld(), shopWidget_bp);
		if (shopWidget) {
			shopWidget->AddToViewport();
		}
	}
}

void ATestPlayerController::MakeMainWidget()
{
	//전투용 메인 위젯을 열어주자.
	if (mainWidget_bp) {
		mainWidget = CreateWidget<UMainWidget>(GetWorld(), mainWidget_bp);
		if (mainWidget) {
			mainWidget->AddToViewport();
		}
	}
}

void ATestPlayerController::SetHP(float value)
{
	if (mainWidget) {
		mainWidget->SetHPBar(value);
	}
}

void ATestPlayerController::SetAmmo(int32 value)
{
	if (mainWidget) {
		mainWidget->SetRemainAmmo(value);
	}
}

void ATestPlayerController::SetMeg(int32 value)
{
	if (mainWidget) {
		mainWidget->SetRemainMeg(value);
	}
}
