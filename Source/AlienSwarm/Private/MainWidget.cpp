// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include "Components/TextBlock.h"

void UMainWidget::SetRemainAmmo(int32 value)
{
	FString ammo = FString::FromInt(value);
	FText ammotext = FText::FromString(ammo);
	RemainAmmo->SetText(ammotext);
}

void UMainWidget::SetRemainMeg(int32 value)
{
	FString meg = FString::FromInt(value);
	FText megtext = FText::FromString(meg);
	RemainMeg->SetText(megtext);
}
