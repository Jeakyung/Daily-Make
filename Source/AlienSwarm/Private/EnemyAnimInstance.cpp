// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include <AlienEnemy.h>

void UEnemyAnimInstance::NativeInitializeAnimation()
{

	Super::NativeInitializeAnimation();

	enemy = Cast<AAlienEnemy>(GetOwningActor());

}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (enemy != nullptr)
	{
		if (enemy->bMoveAnim)
		{
			bMove = true;
		}
		else
		{
			bMove = false;
		}

		if (enemy->bAttackAnim)
		{
			bAttack = true;
		}
		else
		{
			bAttack = false;
		}
	}

}
