// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Engine/World.h"
#include <AlienEnemy.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>
#include "Components/PrimitiveComponent.h"
#include <../../../../../../../Source/Runtime/Core/Public/Delegates/Delegate.h>
#include <../AlienSwarmCharacter.h>


// Sets default values
AEnemySpawner::AEnemySpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(sceneComp);

	overlapCheckBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Overlap Check Box"));
	overlapCheckBox->SetupAttachment(RootComponent);
	overlapCheckBox->SetGenerateOverlapEvents(true);
	overlapCheckBox->SetBoxExtent(FVector(100.f, 100.f, 50.f));

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	// SpawnEnemy();
	overlapCheckBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemySpawner::PlayerOverlap);
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bSpawnInfinity){
		SpawnEnemy();
	}
	else if (bOverlapToComp)
	{
		if (currentTime < SpawnTime)
		{
			currentTime += DeltaTime;
		}
		else if (currentTime >= DeltaTime) {
			SpawnEnemy();
			currentTime = 0;
			EnemyCount++;

			if(EnemyCount >MaxEnemyCount){
				bOverlapToComp = false;
			}
		}
	}

	

}

void AEnemySpawner::SpawnEnemy()
{
	// ó������ ��ġ�� �����ϰ� �������ʹ� ������ ��
	FVector loc = GetActorLocation();
	FRotator rot = GetActorRotation();

	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<AAlienEnemy>(enemy, loc, rot, params);

}

void AEnemySpawner::PlayerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// �ε��� ����� �÷��̾��� ���ʹ� ����
	player = Cast<AAlienSwarmCharacter>(OtherActor);

	if (player)
	{
		bOverlapToComp = true;
		UE_LOG(LogTemp, Warning, TEXT("Overlaped on Player"));
		// SpawnEnemy();
	}
	
}



