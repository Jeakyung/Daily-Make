// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include <AlienEnemy.h>

void UEnemyAnimInstance::NativeInitializeAnimation()
{

	Super::NativeInitializeAnimation();

	enemy = Cast<AAlienEnemy>(GetOwningActor());

	if (enemy != nullptr)
	{

	}
}
