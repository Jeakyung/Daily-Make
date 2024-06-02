// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "ToolHealPack.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSWARM_API AToolHealPack : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	AToolHealPack();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	bool bCanFire = true;
	float coolDown = 5.0f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AToolHealPack> healPack_BP;

	UPROPERTY(EditAnywhere)
	bool bSet = false;

	UPROPERTY()
	float currentTime = 0.0f;

	float healDuration = 5.0f;

	float healedTime = 0.0f;

	float heal;
	//void CheckOwner();

	virtual bool OnFire(FVector mousePos) override;

	void HealCheak();
};
