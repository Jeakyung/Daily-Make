// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

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

void UMainWidget::Setimages(UTexture2D* image1, UTexture2D* image2, UTexture2D* image3)
{
	if (image1 != nullptr) {
		Weapon1->SetBrushFromTexture(image1);
	}
	if (image2 != nullptr) {
		Weapon2->SetBrushFromTexture(image2);
	}
	if (image3 != nullptr) {
		SubWeapon->SetBrushFromTexture(image3);
	}
}
