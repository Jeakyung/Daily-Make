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


	// 추적할 타겟 검색
	void SearchForTarget();

	// 최대체력
	UPROPERTY(EditAnywhere, Category = Enemy)
	int32 maxHP;
	// 현재체력
	UPROPERTY(EditAnywhere, Category = Enemy)
	int32 currentHP;

	// 공격력
	UPROPERTY(EditAnywhere, Category = Enemy)
	int32 enemyCP = 30;
	
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

	// 문에 부딪치다
	void HitTheDoor();

};
