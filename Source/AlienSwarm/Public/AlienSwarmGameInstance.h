// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AlienSwarmGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSWARM_API UAlienSwarmGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 totalCurrency = 20000;

};
