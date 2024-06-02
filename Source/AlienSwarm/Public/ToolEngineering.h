// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "ToolEngineering.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSWARM_API AToolEngineering : public AWeaponBase
{
	GENERATED_BODY()

public:
	AToolEngineering();

	virtual bool OnFire(FVector mousePos) override;
	
};
