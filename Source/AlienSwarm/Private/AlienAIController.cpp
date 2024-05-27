#include "AlienAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "AlienSwarm/AlienSwarmCharacter.h"
#include "AlienEnemy.h"

AAlienAIController::AAlienAIController()
{
	// FActorTick함수 PrimaryActorTick
	// TickActor() 를 호출하는 기본 액터 틱 함수. 틱 기능은 틱 활성화 여부, 프레임 중 업데이트 발생 시점을 제어하고 틱 종속성을 설정하도록 구성할 수 있다
	PrimaryActorTick.bCanEverTick = true;

	AIPerComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));


}

void AAlienAIController::BeginPlay()
{
	Super::BeginPlay();

	enemy = Cast<AAlienEnemy>(AController::GetPawn());

}

void AAlienAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AAlienAIController::MoveToTarget(AAlienSwarmCharacter* target)
{
	// 타겟이 존재한다면 타겟을 향해 이동한다
	if (enemy)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *enemy->GetName());
	}
	if (target)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *target->GetName());
	}

	// 방향설정
	targetDirection = target->GetActorLocation() - enemy->GetActorLocation();
	targetDirection.Z = 0;

	// 타겟을 바라보도록 회전한다
	enemy->SetActorRotation(targetDirection.ToOrientationRotator());

	// 거리가 공격 가능 범위보다 작다면 공격한다
	if (targetDirection.Size() < attakDistance)
	{
		Attack();
	}
	// 거리가 공격 가능 범위보다 크다면 타겟에게 다가간다
	else
	{
		enemy->AddMovementInput(targetDirection.GetSafeNormal());
		
	}

}

void AAlienAIController::Attack()
{
	// 공격할 대상이 문인지 플레이어인지 확인한다
	// 문이라면

	// 플레이어라면
	// 
}

void AAlienAIController::OnDamaged()
{
}

void AAlienAIController::Die()
{
}

