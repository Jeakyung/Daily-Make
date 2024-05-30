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

void UEnemyAnimInstance::AnimNotify_PrepareAttack()
{
	bEnemyDisableMovement = true;
	currentEnemyLoc = enemy->GetActorLocation();
}

void UEnemyAnimInstance::AnimNotify_AttackStart()
{
}

void UEnemyAnimInstance::AnimNotify_AttackEnd()
{	
	bEnemyDisableMovement = false;
	// enemy->TargetDistCheck(enemy->myTarget);
}

void UEnemyAnimInstance::AnimNotify_AttackPoint()
{
	// 플레이어 체력 손실

}

