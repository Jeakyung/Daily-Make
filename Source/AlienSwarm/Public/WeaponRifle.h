// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "WeaponRifle.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSWARM_API AWeaponRifle : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	virtual bool OnFire(FVector mousePos) override;

};
