#include "AlienAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "AlienSwarm/AlienSwarmCharacter.h"
#include "AlienEnemy.h"

AAlienAIController::AAlienAIController()
{
	// FActorTick�Լ� PrimaryActorTick
	// TickActor() �� ȣ���ϴ� �⺻ ���� ƽ �Լ�. ƽ ����� ƽ Ȱ��ȭ ����, ������ �� ������Ʈ �߻� ������ �����ϰ� ƽ ���Ӽ��� �����ϵ��� ������ �� �ִ�
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
	// Ÿ���� �����Ѵٸ� Ÿ���� ���� �̵��Ѵ�
	if (enemy)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *enemy->GetName());
	}
	if (target)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *target->GetName());
	}

	// ���⼳��
	targetDirection = target->GetActorLocation() - enemy->GetActorLocation();
	targetDirection.Z = 0;

	// Ÿ���� �ٶ󺸵��� ȸ���Ѵ�
	enemy->SetActorRotation(targetDirection.ToOrientationRotator());

	// �Ÿ��� ���� ���� �������� �۴ٸ� �����Ѵ�
	if (targetDirection.Size() < attakDistance)
	{
		Attack();
	}
	// �Ÿ��� ���� ���� �������� ũ�ٸ� Ÿ�ٿ��� �ٰ�����
	else
	{
		enemy->AddMovementInput(targetDirection.GetSafeNormal());
		
	}

}

void AAlienAIController::Attack()
{
	// ������ ����� ������ �÷��̾����� Ȯ���Ѵ�
	// ���̶��

	// �÷��̾���
	// 
}

void AAlienAIController::OnDamaged()
{
}

void AAlienAIController::Die()
{
}

