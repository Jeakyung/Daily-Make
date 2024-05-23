<<<<<<< HEAD

=======
// Fill out your copyright notice in the Description page of Project Settings.
>>>>>>> f941cead1e822f0237595fed37505f29f91a73d8

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
<<<<<<< HEAD
#include "PlayerAnimInstance.generated.h"


UCLASS()
class ALIENSWARM_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	public:
=======
#include "MyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSWARM_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
>>>>>>> f941cead1e822f0237595fed37505f29f91a73d8

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UPROPERTY()
	class AAlienSwarmCharacter* player;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Horizontal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Vertical;

<<<<<<< HEAD
=======
	
	
>>>>>>> f941cead1e822f0237595fed37505f29f91a73d8
};
