// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Engine/World.h"
#include <AlienEnemy.h>

// Sets default values
AEnemySpawner::AEnemySpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnEnemy();

}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (currentTime < SpawnTime)
	{
		currentTime += DeltaTime;
	}
	else if (currentTime >= DeltaTime) {
		SpawnEnemy();
		currentTime = 0;
	}

}

void AEnemySpawner::SpawnEnemy()
{
	if (MaxEnemyCount == ++EnemyCount)
	{
		Destroy();
	}

	// 처음에만 위치값 저장하고 다음부터는 가져다 씀
	static FVector loc = GetActorLocation();
	static FRotator rot = GetActorRotation();

	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<AAlienEnemy>(enemy, loc, rot, params);

}

