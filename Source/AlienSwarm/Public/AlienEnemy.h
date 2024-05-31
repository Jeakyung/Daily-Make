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
	
	UPROPERTY()
	class USphereComponent* sphereCollision;
	
	/*UPROPERTY()
	class AGeometryCollectionActor* explosionEnemy;*/


	// 실시간 타겟과의 거리 비교
	void TargetDistCheck(AAlienSwarmCharacter* target);

	// 최대체력
	UPROPERTY(EditAnywhere, Category = Enemy)
	int32 maxHP=30;
	// 현재체력
	UPROPERTY(EditAnywhere, Category = Enemy)
	int32 currentHP=maxHP;

	// 공격력
	UPROPERTY(EditAnywhere, Category = Enemy)
	int32 enemyCP = 300;
	
	//이동 속도
	UPROPERTY(EditAnywhere, Category = Enemy)
	float traceSpeed = 720.f;

	TArray<AAlienSwarmCharacter*> targetList;

	// 타겟과 에너미 사이에 문이 존재하는지 체크
	UFUNCTION()
	void TargetCheck();

	bool bHasTarget = false;

	UPROPERTY()
	class AAlienAIController* AIEnemyController;
	
	// 공격 가능 거리	
	UPROPERTY(EditAnywhere, Category = Enemy)
	bool bHitTheDoor = false;

	// 공격 가능 거리	
	UPROPERTY(EditAnywhere, Category = Enemy)
	float attakDistance = 170.f;
	
	// 현재 타켓과 에너미 사이의 거리
	UPROPERTY(EditAnywhere, Category = Enemy)
	float CurrentDistance;

	// 문이 열려 있는지 확인
	UFUNCTION()
	void DoorStateCheck();

	// 문 공격
	UFUNCTION()
	void AttackDoor();

	// 타겟 플레이어에게 데미지 가함
	UFUNCTION()
	void DoDamageToTargetPlayer();

	virtual void TakeHit(int32 damage) override;


public:
	// 애니메이션
	
	// 이동 애니메이션
	UPROPERTY(EditAnywhere, Category = Enemy)
	bool bMoveAnim = false;

	// 공격 애니메이션	
	UPROPERTY(EditAnywhere, Category = Enemy)
	bool bAttackAnim = false;

	// 죽음 애니메이션	
	UPROPERTY(EditAnywhere, Category = Enemy)
	bool bDieAnim = false;



};
