// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AlienAIController.h"
#include "AlienEnemy.generated.h"

UCLASS()
class ALIENSWARM_API AAlienEnemy : public ACharacter
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


	// ������ Ÿ�� �˻�
	void SearchForTarget();

	// �ִ�ü��
	UPROPERTY(EditAnywhere, Category = Enemy)
	int32 maxHP;
	// ����ü��
	UPROPERTY(EditAnywhere, Category = Enemy)
	int32 currentHP;

	// ���ݷ�
	UPROPERTY(EditAnywhere, Category = Enemy)
	int32 enemyCP = 30;
	
	//�̵� �ӵ�
	UPROPERTY(EditAnywhere, Category = Enemy)
	float traceSpeed = 720.f;

	TArray<AAlienSwarmCharacter*> targetList;

	// Ÿ�ٰ� ���ʹ� ���̿� ���� �����ϴ��� üũ
	UFUNCTION()
	void TargetCheck();

	bool bHasTarget = false;

	UPROPERTY()
	class AAlienAIController* AIEnemyController;

	// ���� �ε�ġ��
	void HitTheDoor();

};
