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
	ATestPlayerController* pc = Cast<ATestPlayerController>(GetOwningPlayer());

	pc->MakeMainWidget();
	pc->MoveToStartPos();

	ConditionalBeginDestroy();
}
