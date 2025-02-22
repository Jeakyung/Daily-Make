#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AlienAIController.generated.h"

/**
 * 
 */

 class UBehaviorTreeComponent;
 class UBlackboardComponent;

UCLASS()
class ALIENSWARM_API AAlienAIController : public AAIController
{
	GENERATED_BODY()

public:
	AAlienAIController();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, Category = Enemy)
	class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(EditDefaultsOnly, Category = Enemy)
	class UAIPerceptionComponent* AIPerComp;


private:
	UPROPERTY(EditDefaultsOnly)
	UBlackboardComponent* BlackboardComp;

	UPROPERTY(EditDefaultsOnly)
	UBehaviorTreeComponent* BehaviorComp;

public:
	// 타겟 방향
	UPROPERTY(EditDefaultsOnly)
	FVector targetDirection;	
	// 공격 가능 거리	
	UPROPERTY(EditAnywhere, Category = Enemy)
	float attakDistance = 150.f;
	// 추적 속력
	//UPROPERTY(EditAnywhere, Category = Enemy)
	//float attakDistance = ;
	
	UPROPERTY(EditDefaultsOnly)
	class AAlienSwarmCharacter* myTarget;
	UPROPERTY()
	class AAlienEnemy* enemy;
	
	UFUNCTION()
	// 타겟을 향해 이동
	void MoveToTarget(AAlienSwarmCharacter* target);
	
	// 타겟 공격
	// 타겟 대상에 따른 분기는 enemy.cpp에서 구현
	// attack delay는 enemy.cpp에서 구현
	void Attack();

	// 피격데미지 처리
	void OnDamaged();

	// 죽음
	void Die();

	// 문 열려있는지 체크
	void DoorCheck();

	UPROPERTY(EditAnywhere, Category = Enemy)
	bool bDoorOpen = false;
	
	UPROPERTY(EditAnywhere, Category = Enemy)
	bool bAttackDoor = false;

	UPROPERTY(EditAnywhere, Category = Enemy)
	bool bAttackPlayer = false;
};