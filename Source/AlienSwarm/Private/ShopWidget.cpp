// Fill out your copyright notice in the Description page of Project Settings.
// 플레이 시작 전 장비 구매 UI.
// 실제 구매처리는 리스트뷰에서 추가되는 블루프린트로 만든 위젯에서 실시하고 있다.

#include "ShopWidget.h"
#include "Components/Button.h"
#include "TestPlayerController.h"
#include "../AlienSwarmCharacter.h"


void UShopWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_Start->OnReleased.AddDynamic(this, &UShopWidget::StartGame);
}

void UShopWidget::StartGame()
{
	
	ServerRPC_Test();
	


	ConditionalBeginDestroy();
}

void UShopWidget::ServerRPC_Test_Implementation()
{
	MultiRPC_Test();
}

void UShopWidget::MultiRPC_Test_Implementation()
{
	ATestPlayerController* pc = Cast<ATestPlayerController>(GetOwningPlayer());

	pc->MakeMainWidget();
	pc->MoveToStartPos();

	AAlienSwarmCharacter* playerREF = Cast<AAlienSwarmCharacter>(pc->GetPawn());
	playerREF->ServerRPC_SpawnWeapon();
	playerREF->ChangeWeapon(playerREF->Weapon);
}
