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
#include "Net/UnrealNetwork.h"
#include <ExplosionEnemyBody.h>
#include "Engine/World.h"
#include "DoorActor.h"

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

	bReplicates = true;
	SetReplicateMovement(true);

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
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HAVE CONTROLLER"));
	}

	// explosionEnemy = Cast<AExplosionEnemyBody>(GetActorClassDefaultComponent);

}

// Called every frame
void AAlienEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("target<->enemy dist : %f"), CurrentDistance);

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

	if (!bIsTargetDoor)
	{
		// �����ð����� Ÿ�� ��Ž��

		if (curTime < targetResetTime)
		{
			curTime += DeltaTime;

		}
		else if (targetResetTime <= curTime)
		{
			curTime = 0.f;
			TargetCheck();
			//EnemyDie();
			UE_LOG(LogTemp, Warning, TEXT("22"));
		}

	}
	else
	{
		AttackDoor();
	}

	if (bisAttackPoint)
	{
		ServerRPC_DoDamageToTargetPlayer();
		bisAttackPoint = false;
	}

	if (1)
	{
		//UEnum::GetValueAsString();



		FString str = FString::Printf(TEXT("enemyAnim->bEnemyDisableMovement : %s"), enemyAnim->bEnemyDisableMovement ? L"TRUE" : L"FALSE");

		FVector loc = GetActorLocation() + FVector(0, 0, 50);
		//DrawDebugString(GetWorld(), loc, str, nullptr, FColor::White, 0, true);
	}
}

// Called to bind functionality to input
void AAlienEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAlienEnemy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// ���ʹ� ü�� ����ȭ
	DOREPLIFETIME(AAlienEnemy, currentHP);
}

void AAlienEnemy::TargetDistCheck(AAlienSwarmCharacter* target)
{
	if (target != nullptr)
	{
		CurrentDistance = (target->GetActorLocation() - GetActorLocation()).Size();
	}
	//AIEnemyController->MoveToActor(target);
	// �Ÿ��� ���� ���� �������� �۴ٸ� �����Ѵ�
	if (CurrentDistance < attakDistance)
	{
		bMoveAnim = false;
		bAttackAnim = true;
		// AIEnemyController->Attack();
		//���⼭ �������� �ٲٱ�
	}
	// �Ÿ��� ���� ���� �������� ũ�ٸ� Ÿ�ٿ��� �ٰ�����
	else
	{
		//if (bAttackAnim == false)
		bAttackAnim = false;
		bMoveAnim = true;

		// Ÿ���� ���� �̵�
		//AIEnemyController->MoveToActor(target);

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

	targetList.Empty();

	for (TActorIterator<AAlienSwarmCharacter> target(GetWorld()); target; ++target)
	{
		//myTarget = *target;
		targetList.Add(*target);
	}

	if (targetList.Num() > 0)
	{
		FVector nearestDistance = targetList[0]->GetActorLocation() - GetActorLocation();
		float nearestDistanceLength = nearestDistance.Size();

		/*UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"), nearestDistance.X, nearestDistance.Y, nearestDistance.Z);
		UE_LOG(LogTemp, Warning, TEXT("closestDistanceLength: %f"), nearestDistanceLength);*/


		// ���� ������ �ִ� Ÿ�� Ž��
		// for (AAlienSwarmCharacter* target : targetList)
		// {
		//		UE_LOG(LogTemp, Warning, TEXT("11111%s\n"), *target->GetName());
		// }

		int32 nearestTargetIndex = 0;

		if (targetList.Num() > 0)
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

			/*UE_LOG(LogTemp, Warning, TEXT("Finally closestDistanceLength: %f"), nearestDistanceLength);
			UE_LOG(LogTemp, Warning, TEXT("Finally closestTargetIndex: %d"), nearestTargetIndex);*/
		}
		// ���� �����̿� �ִ� �÷��̾ Ÿ������ ����
		myTarget = targetList[nearestTargetIndex];
		if (AIEnemyController)
			AIEnemyController->MoveToActor(myTarget, 70.0f);
		TargetDistCheck(myTarget);

		// UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Index : %d, Target : %s"), nearestTargetIndex, *myTarget->GetActorNameOrLabel()));

		// CurrentDistance = (myTarget->GetActorLocation() - GetActorLocation()).Size();


	}

}

void AAlienEnemy::DoorStateCheck()
{
}

void AAlienEnemy::AttackDoor()
{
	if (AIEnemyController)
	{
		AIEnemyController->StopMovement();
	}
	
	bMoveAnim = false;
	bAttackAnim = true;

	// �ڿ��� �ǰ��� ���ߴٸ�
	if (bAttackedBack)
	{
		// enemyAnim->bAttackedBack = true;
		// myTarget = �ڿ��� ���� ������ �÷��̾�
		TargetDistCheck(myTarget);
	}
}

void AAlienEnemy::TakeHit(int32 damage)
{
	UE_LOG(LogTemp, Warning, TEXT("takeHitFunc"));
	UE_LOG(LogTemp, Error, TEXT("enemyHP: %d"), currentHP);


	currentHP -= damage;
	SetOwner(GetWorld()->GetFirstPlayerController());
	ServerRPC_TakeDamage(currentHP);

}

void AAlienEnemy::MultiRPC_SetHP_Implementation(int32 hp)
{
	UE_LOG(LogTemp, Warning, TEXT("MultiSerHPFunc"));
	currentHP = hp;
}

void AAlienEnemy::ServerRPC_TakeDamage_Implementation(int32 hp)
{
	UE_LOG(LogTemp, Warning, TEXT("ServerTakeDamageFunc"));

	currentHP = hp;
	UE_LOG(LogTemp, Warning, TEXT("enemyHP: %d"), currentHP);

	MultiRPC_SetHP(currentHP);


	if (currentHP <= 0)
	{
		MultiRPC_EnemyDie();

		UE_LOG(LogTemp, Warning, TEXT("DieEnemy"));
	}
	// UE_LOG(LogTemp, Warning, TEXT("HitDamege"));
}

void AAlienEnemy::ServerRPC_DoDamageToTargetPlayer_Implementation()
{
	if (!bIsTargetDoor && myTarget)
	{
		FVector playerLoc = myTarget->GetActorLocation();
		if ((playerLoc - GetActorLocation()).Size() < attakDistance)
		{
			UE_LOG(LogTemp, Warning, TEXT("aaattttttaaaccckkkk"));

			IHitInterface* targetPlayer = Cast<IHitInterface>(myTarget);
			if (targetPlayer)
			{
				UE_LOG(LogTemp, Warning, TEXT("targetPlayer HP -100"));

				targetPlayer->TakeHit(enemyCP);
			}
		}

	}
	else if (bIsTargetDoor && myDoorTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("distover"));


		//FVector playerLoc = myDoorTarget->GetActorLocation();
		//if ((playerLoc - GetActorLocation()).Size() < attakDistance)
		{
			UE_LOG(LogTemp, Warning, TEXT("aaattttttaaaccckkkk"));

			IHitInterface* targetPlayer = Cast<IHitInterface>(myDoorTarget);
			if (targetPlayer)
			{
				UE_LOG(LogTemp, Warning, TEXT("targetPlayer HP -100"));

				targetPlayer->TakeHit(enemyCP);
			}
		}
	}
	else
	{
		if (bIsTargetDoor) {
			UE_LOG(LogTemp, Warning, TEXT("bistargetdoor"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("NOTbistargetdoor"));
		}
		if (myDoorTarget) {
			UE_LOG(LogTemp, Warning, TEXT("mydoortarget"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("NOTbistargetdoor"));
		}
	}
	//�̵� ������Ʈ�� �ٲٱ�DoDamageToTargetPlayer
	//if(bAttackAnim = false && bMoveAnim = true){
	//	//���⼭ ���� �� ���� �ٲٱ�
	//}
}


void AAlienEnemy::MultiRPC_EnemyDie_Implementation()
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

