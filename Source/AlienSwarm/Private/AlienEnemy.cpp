// Fill out your copyright notice in the Description page of Project Settings.


#include "AlienEnemy.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include <AlienAIController.h>

// Sets default values
AAlienEnemy::AAlienEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	SearchForTarget();

}

// Called when the game starts or when spawned
void AAlienEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	AIEnemyController = Cast<AAlienAIController>(GetController());
	if (AIEnemyController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("NULL"));
	}
	
	TargetCheck();
}

// Called every frame
void AAlienEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAlienEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAlienEnemy::SearchForTarget()
{
}

void AAlienEnemy::TargetCheck()
{
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AActor::StaticClass(), TEXT("target"), targetList);
	// for (TActorIterator<AActor> target(GetWorld()); target; ++target)
	/*{
		myTarget = *target;
	}*/
	if (targetList.Num() > 0) {
		myTarget = targetList[0];
		AIEnemyController->MoveToTarget(myTarget);

	}

	if (AIEnemyController != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("asadfgg"));
	}
}

