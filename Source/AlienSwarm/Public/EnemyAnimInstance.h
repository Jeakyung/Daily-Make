// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSWARM_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Enemy)
	class AAlienEnemy* enemy;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Enemy)
	bool bMove = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Enemy)
	bool bAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Enemy)
	FVector currentVelocity;
		
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UFUNCTION()
	void AnimNotify_PrepareAttack();
	UFUNCTION()
	void AnimNotify_AttackStart();
	UFUNCTION()
	void AnimNotify_AttackEnd();
	UFUNCTION()
	void AnimNotify_AttackPoint();
	

	UPROPERTY(EditAnywhere)
	bool bEnemyDisableMovement = false;
	
	UPROPERTY(EditDefaultsOnly)
	FVector currentEnemyLoc;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Enemy)
	bool bAttackedBack = false;
};
