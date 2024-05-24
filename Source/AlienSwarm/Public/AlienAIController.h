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
	// Ÿ�� ����
	FVector targetDirection;	
	// ���� ���� �Ÿ�	
	UPROPERTY(EditAnywhere, Category = Enemy)
	float attakDistance = 150.f;
	// ���� �ӷ�
	//UPROPERTY(EditAnywhere, Category = Enemy)
	//float attakDistance = ;

	AActor* myTarget;
	class AAlienEnemy* enemy;
	
	UFUNCTION()
	// Ÿ���� ���� �̵�
	void MoveToTarget(AActor* target);
	
	// Ÿ�� ����
	// Ÿ�� ��� ���� �б�� enemy.cpp���� ����
	// attack delay�� enemy.cpp���� ����
	void Attack();

	// �ǰݵ����� ó��
	void OnDamaged();

	// ����
	void Die();

};