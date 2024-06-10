// Fill out your copyright notice in the Description page of Project Settings.


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
