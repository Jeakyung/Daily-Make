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
		bMove = enemy->bMoveAnim;

		bAttack = enemy->bAttackAnim;
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

	enemy->bAttackAnim = false;

	// enemy->TargetDistCheck(enemy->myTarget);
	// 여기에서 이동 스테이트로 변경
}

void UEnemyAnimInstance::AnimNotify_AttackPoint()
{
	// 플레이어 체력 손실
	enemy->ServerRPC_DoDamageToTargetPlayer();
}

