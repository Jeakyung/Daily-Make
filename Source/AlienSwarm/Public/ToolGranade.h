// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "ToolGranade.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSWARM_API AToolGranade : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	virtual bool OnFire(FVector mousePos) override;

	bool bCanFire = true;
	float coolDown = 5.0f;
};
