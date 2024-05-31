// Fill out your copyright notice in the Description page of Project Settings.


#include "TestPlayerController.h"
#include "TitleWidget.h"
#include "ShopWidget.h"
#include "MainWidget.h"
#include "../AlienSwarmCharacter.h"

void ATestPlayerController::BeginPlay()
{
	
}

void ATestPlayerController::MakeTitleWidget()
{
	//Ÿ��Ʋ ������ ��������
	if (titleWidget_bp) {
		titleWidget = CreateWidget<UTitleWidget>(GetWorld(), titleWidget_bp);
		if (titleWidget) {
			titleWidget->AddToViewport();
			SetInputMode(FInputModeUIOnly());
			SetShowMouseCursor(true);
		}
	}
}

void ATestPlayerController::MakeShopWidget()
{
	//�� ������ ��������.
	if (shopWidget_bp) {
		shopWidget = CreateWidget<UShopWidget>(GetWorld(), shopWidget_bp);
		if (shopWidget) {
			shopWidget->AddToViewport();
			SetInputMode(FInputModeUIOnly());
			SetShowMouseCursor(true);
		}
	}
}

void ATestPlayerController::MakeMainWidget()
{
	//������ ���� ������ ��������.
	if (mainWidget_bp) {
		mainWidget = CreateWidget<UMainWidget>(GetWorld(), mainWidget_bp);
		if (mainWidget) {
			mainWidget->Setimages(Weapon1, Weapon2, SubWeapon);
			mainWidget->AddToViewport();

			SetInputMode(FInputModeGameOnly());
			SetShowMouseCursor(false);
		}
	}
}

void ATestPlayerController::MoveToStartPos()
{
	ServerRPC_MovePos();
}

void ATestPlayerController::ServerRPC_MovePos_Implementation()
{
	FVector basePos = FVector(-20000.0f, 0.0f, 101.0f);
	basePos.X += FMath::RandRange(-200.0f, 200.0f);
	basePos.Y += FMath::RandRange(-500.0f, 500.0f);
	MultiRPC_MovePos(basePos);
}

void ATestPlayerController::MultiRPC_MovePos_Implementation(FVector MovePos)
{
	AAlienSwarmCharacter* playerREF = Cast< AAlienSwarmCharacter>(GetPawn());
	playerREF->SetActorLocation(MovePos);
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
