// Fill out your copyright notice in the Description page of Project Settings.


#include "AlienEnemy.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include <AlienAIController.h>
#include "AlienSwarm/AlienSwarmCharacter.h"

// Sets default values
AAlienEnemy::AAlienEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

}

// Called when the game starts or when spawned
void AAlienEnemy::BeginPlay()
{
	Super::BeginPlay();

	AIEnemyController = Cast<AAlienAIController>(GetController());

	if (AIEnemyController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("NULL"));
	}

	TargetCheck();
}

// Called every frame
void AAlienEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (myTarget != nullptr)
	{
		TargetDistCheck(myTarget);
	}

}

// Called to bind functionality to input
void AAlienEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAlienEnemy::TargetDistCheck(AAlienSwarmCharacter* target)
{
	CurrentDistance = (target->GetActorLocation() - GetActorLocation()).Size();

	// �Ÿ��� ���� ���� �������� �۴ٸ� �����Ѵ�
	if (CurrentDistance < attakDistance)
	{
		bMoveAnim = false;
		bAttackAnim = true;
		AIEnemyController->Attack();
	}
	// �Ÿ��� ���� ���� �������� ũ�ٸ� Ÿ�ٿ��� �ٰ�����
	else
	{
		bAttackAnim = false;
		// Ÿ���� ���� �̵�
		bMoveAnim = true;
		AIEnemyController->MoveToActor(target);

		if (bHitTheDoor)
		{
			AIEnemyController->DoorCheck();
		}

	}

}

void AAlienEnemy::TargetCheck()
{
	for (TActorIterator<AAlienSwarmCharacter> target(GetWorld()); target; ++target)
	{
		// myTarget = *target;
		targetList.Add(*target);
	}

	FVector nearestDistance = targetList[0]->GetActorLocation() - GetActorLocation();
	float nearestDistanceLength = nearestDistance.Size();

	UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"), nearestDistance.X, nearestDistance.Y, nearestDistance.Z);
	UE_LOG(LogTemp, Warning, TEXT("closestDistanceLength: %f"), nearestDistanceLength);


	// ���� ������ �ִ� Ÿ�� Ž��
	// for (AAlienSwarmCharacter* target : targetList)
	// {
	//		UE_LOG(LogTemp, Warning, TEXT("11111%s\n"), *target->GetName());
	// }

	int32 nearestTargetIndex = 0;

	if (targetList.Num() > 1)
	{
		FVector targetDistance;

		for (int i = 1; i < targetList.Num(); i++)
		{
			targetDistance = targetList[i]->GetActorLocation() - GetActorLocation();
			float targetDistanceLength = targetDistance.Size();
			UE_LOG(LogTemp, Warning, TEXT("targetIndex: %d, targetDistanceLength: %f\n"), i, targetDistanceLength);

			if (targetDistanceLength < nearestDistanceLength)
			{
				nearestDistanceLength = targetDistanceLength;
				nearestTargetIndex = i;
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("Finally closestDistanceLength: %f"), nearestDistanceLength);
		UE_LOG(LogTemp, Warning, TEXT("Finally closestTargetIndex: %d"), nearestTargetIndex);

		// ���� �����̿� �ִ� �÷��̾ Ÿ������ ����
		myTarget = targetList[nearestTargetIndex];
		// CurrentDistance = (myTarget->GetActorLocation() - GetActorLocation()).Size();


	}
}



