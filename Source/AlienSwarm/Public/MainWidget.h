// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSWARM_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UImage* Weapon1;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UImage* Weapon2;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UImage* SubWeapon;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UProgressBar* HPBar;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* RemainAmmo;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* RemainMeg;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	float HPPercent = 1.0f;

	FORCEINLINE void SetHPBar(float value) {HPPercent = value;};

	void SetRemainAmmo(int32 value);

	void SetRemainMeg(int32 value);
};
