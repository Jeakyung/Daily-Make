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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UEnemyAnimInstance* enemyAnim;

	UPROPERTY()
	class ADoorActor* doorActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Comp")
	class USphereComponent* sphereCollision;
	
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Comp")
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AExplosionEnemyBody> explosionEnemy;

	TArray<AAlienSwarmCharacter*> targetList;
	
	// �ִ�ü��
	UPROPERTY(EditAnywhere, Category = Enemy)
	int32 maxHP=30;
	// ����ü��
	UPROPERTY(EditAnywhere, Category = Enemy)
	int32 currentHP=maxHP;

	// ���ݷ�
	UPROPERTY(EditAnywhere, Category = Enemy)
	int32 enemyCP = 100;
	
	//�̵� �ӵ�
	UPROPERTY(EditAnywhere, Category = Enemy)
	float traceSpeed = 720.f;

		// Ÿ�� �缳�� �ð�
	float curTime=0.f;
	float targetResetTime = 2.f;

	// �� ���� ����
	bool bAttackDoor = false;

	bool bStopMovement = false;

	bool bHasTarget = false;
	
	// ���� ���� �Ÿ�	
	UPROPERTY(EditAnywhere, Category = Enemy)
	bool bHitTheDoor = false;

	// ���� ���� �Ÿ�	
	UPROPERTY(EditAnywhere, Category = Enemy)
	float attakDistance = 150.f;
	
	// ���� Ÿ�ϰ� ���ʹ� ������ �Ÿ�
	UPROPERTY(EditAnywhere, Category = Enemy)
	float CurrentDistance;


	UPROPERTY()
	class AAlienAIController* AIEnemyController;

	UFUNCTION()
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
	UFUNCTION(Server, Reliable)
	void ServerRPC_DoDamageToTargetPlayer();

	// takeHit�� �״�� �޾ƿ���
	virtual void TakeHit(int32 damage) override;

	// �޾ƿ� takeHit �������̽� �������� ���� 
	UFUNCTION(Server, Reliable)
	void ServerRPC_TakeDamage(int32 damage);


	// ������ �� ����
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_EnemyDie();


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
