// Fill out your copyright notice in the Description page of Project Settings.


#include "AlienEnemy.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/SphereComponent.h>
#include "GeometryCollection/GeometryCollectionComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include <AlienAIController.h>
#include "AlienSwarm/AlienSwarmCharacter.h"
#include <EnemyAnimInstance.h>
#include "HitInterface.h"
#include <ExplosionEnemyBody.h>
#include "Engine/World.h"

// Sets default values
AAlienEnemy::AAlienEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	sphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("sphereCollision"));
	sphereCollision->SetupAttachment(RootComponent);
	sphereCollision->SetSphereRadius(80.f);

	// explosionComp = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("explosionComp"));
	// explosionComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AAlienEnemy::BeginPlay()
{
	Super::BeginPlay();

	AIEnemyController = Cast<AAlienAIController>(GetController());
	enemyAnim = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());


	TargetCheck();

	// sphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ADoorActor::EnemyAttackDoor);

	if (AIEnemyController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("NULL"));
	}

	// explosionEnemy = Cast<AExplosionEnemyBody>(GetActorClassDefaultComponent);

}

// Called every frame
void AAlienEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (myTarget != nullptr)
	{
		TargetDistCheck(myTarget);
	}

	if (enemyAnim != nullptr)
	{
		if (enemyAnim->bEnemyDisableMovement)
		{
			SetActorLocation(enemyAnim->currentEnemyLoc);
			// AIEnemyController->StopMovement();

		}
		else
		{
			TargetDistCheck(myTarget);
		}
	}

	if (!bAttackDoor)
	{
		// �����ð����� Ÿ�� ��Ž��
		if (curTime < targetResetTime)
		{
			curTime += DeltaTime;
		}
		else if (targetResetTime <= curTime)
		{
			curTime = 0.f;
			// TargetCheck();
			EnemyDie();
		}
	}
	else
	{
		AttackDoor();
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
		bMoveAnim = true;
		// Ÿ���� ���� �̵�
		AIEnemyController->MoveToActor(target);

		if (bHitTheDoor)
		{
			AIEnemyController->DoorCheck();
		}

	}

}

void AAlienEnemy::TargetCheck()
{
	// ���� �������� ���
	/*if ()
	{
		bAttackDoor = true;
		return;
	}*/


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
	}
	// ���� �����̿� �ִ� �÷��̾ Ÿ������ ����
	myTarget = targetList[nearestTargetIndex];
	// CurrentDistance = (myTarget->GetActorLocation() - GetActorLocation()).Size();


}

void AAlienEnemy::DoorStateCheck()
{
	bool bDoorOpen = false;

	// ���� ���� �ִٸ�
	if (!bDoorOpen)
	{
		// ���� �����ؼ� �����
		AttackDoor();
	}
	else
	{

	}
}

void AAlienEnemy::AttackDoor()
{
	// ���� ü���� 0�� �� �� ���� ����

	// ���� ü���� 0�� �� ��� ���� Ÿ���̾��� �÷��̾ �i��
	// if (doorActor->doorHP <= 0)
	{
	}
	// else
	{
		// �ڿ��� �ǰ��� ���ߴٸ�
		// if (TakeHit) �� �ڵ� err
		{
			enemyAnim->bAttackedBack = true;

		}
	}
}

void AAlienEnemy::DoDamageToTargetPlayer()
{
	if (myTarget)
	{
		FVector playerLoc = myTarget->GetActorLocation();
		if ((playerLoc - GetActorLocation()).Size() < attakDistance)
		{
			UE_LOG(LogTemp, Warning, TEXT("aaattttttaaaccckkkk"));

			IHitInterface* targetPlayer = Cast<IHitInterface>(myTarget);
			if (targetPlayer)
			{
				UE_LOG(LogTemp, Warning, TEXT("targetPlayer HP -300"));
				targetPlayer->TakeHit(enemyCP);
			}
		}

	}
}


void AAlienEnemy::TakeHit(int32 damage)
{
	currentHP -= damage;
	UE_LOG(LogTemp, Warning, TEXT("enemyHP: %d"), currentHP);

	if (currentHP <= 0)
	{
		EnemyDie();

		UE_LOG(LogTemp, Warning, TEXT("DieEnemy"));
	}
	// UE_LOG(LogTemp, Warning, TEXT("HitDamege"));
}

void AAlienEnemy::EnemyDie()
{
	FVector loc = GetActorLocation();
	FRotator rot = GetActorRotation();

	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// ������ �޽� ����
	// GetWorld()->SpawnActor<AExplosionEnemyBody>(explosionEnemy, GetActorLocation, GetActorRotation, params);
	auto* spawnedActor = GetWorld()->SpawnActor<AExplosionEnemyBody>(explosionEnemy, loc, rot, params);
	if (spawnedActor != nullptr)
	{
		FVector spawnedActorLoc = spawnedActor->GetActorLocation();
		UE_LOG(LogTemp, Warning, TEXT("SpawnedEnemyLoc; %f, %f, %f"), spawnedActorLoc.X, spawnedActorLoc.Y, spawnedActorLoc.Z);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("nonSpawnd"));

	Destroy();
}

