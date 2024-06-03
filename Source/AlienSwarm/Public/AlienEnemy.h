// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AlienAIController.h"
#include "HitInterface.h"
#include "AlienEnemy.generated.h"

UCLASS()
class ALIENSWARM_API AAlienEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAlienEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY()
	AAlienSwarmCharacter* myTarget;

	UPROPERTY()
	class UEnemyAnimInstance* enemyAnim;

	UPROPERTY()
	class ADoorActor* doorActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Comp")
	class USphereComponent* sphereCollision;
	
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Comp")
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AExplosionEnemyBody> explosionEnemy;

	// �ִ�ü��
	UPROPERTY(EditAnywhere, Category = Enemy)
	int32 maxHP=30;
	// ����ü��
	UPROPERTY(EditAnywhere, Category = Enemy)
	int32 currentHP=maxHP;

	// ���ݷ�
	UPROPERTY(EditAnywhere, Category = Enemy)
	int32 enemyCP = 300;
	
	//�̵� �ӵ�
	UPROPERTY(EditAnywhere, Category = Enemy)
	float traceSpeed = 720.f;

	bool bHasTarget = false;
	
	// ���� ���� �Ÿ�	
	UPROPERTY(EditAnywhere, Category = Enemy)
	bool bHitTheDoor = false;

	// ���� ���� �Ÿ�	
	UPROPERTY(EditAnywhere, Category = Enemy)
	float attakDistance = 130.f;
	
	// ���� Ÿ�ϰ� ���ʹ� ������ �Ÿ�
	UPROPERTY(EditAnywhere, Category = Enemy)
	float CurrentDistance;

	TArray<AAlienSwarmCharacter*> targetList;

	UPROPERTY()
	class AAlienAIController* AIEnemyController;

	// �ǽð� Ÿ�ٰ��� �Ÿ� ��
	void TargetDistCheck(AAlienSwarmCharacter* target);

	// Ÿ�ٰ� ���ʹ� ���̿� ���� �����ϴ��� üũ
	UFUNCTION()
	void TargetCheck();

	// ���� ���� �ִ��� Ȯ��
	UFUNCTION()
	void DoorStateCheck();

	// �� ����
	UFUNCTION()
	void AttackDoor();

	// Ÿ�� �÷��̾�� ������ ����
	UFUNCTION()
	void DoDamageToTargetPlayer();

	virtual void TakeHit(int32 damage) override;

	// ������ �� ����
	UFUNCTION()
	void EnemyDie();

	// Ÿ�� �缳�� �ð�
	float curTime=0.f;
	float targetResetTime = 3.f;

	// �� ���� ����
	bool bAttackDoor = false;



public:
	// �ִϸ��̼�
	
	// �̵� �ִϸ��̼�
	UPROPERTY(EditAnywhere, Category = Enemy)
	bool bMoveAnim = false;

	// ���� �ִϸ��̼�	
	UPROPERTY(EditAnywhere, Category = Enemy)
	bool bAttackAnim = false;

	// ���� �ִϸ��̼�	
	UPROPERTY(EditAnywhere, Category = Enemy)
	bool bDieAnim = false;



};
