// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSWARM_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UPROPERTY()
	class AAlienSwarmCharacter* player;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Horizontal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Vertical;

	
	
};
