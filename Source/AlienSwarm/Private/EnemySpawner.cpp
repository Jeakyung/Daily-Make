// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Engine/World.h"
#include <AlienEnemy.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>
#include "Components/PrimitiveComponent.h"
#include <../../../../../../../Source/Runtime/Core/Public/Delegates/Delegate.h>
#include <../AlienSwarmCharacter.h>
#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"

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
	
	bReplicates = true;
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	// SpawnEnemy();
	overlapCheckBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemySpawner::PlayerOverlap);

	if (HasAuthority()) {
		SetOwner(GetWorld()->GetFirstPlayerController());
	}
	
	playerList.Empty();
	for (TActorIterator<AAlienSwarmCharacter> target(GetWorld()); target; ++target)
	{
		playerList.Add(*target);
	}
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bOverlapToComp || (bSpawnInfinity && spawnDist > CalPlayetDist()))
	{
		if (currentTime < SpawnTime || bSpawnInfinity)
		{
			currentTime += DeltaTime;
		}
		
		if (currentTime > SpawnTime) {
			//SpawnEnemy();
			ServerRPC_SpawnEnemy();
			currentTime = 0;
			EnemyCount++;

			if (EnemyCount > MaxEnemyCount) {
				bOverlapToComp = false;
			}
		}
	}



}

void AEnemySpawner::ServerRPC_SpawnEnemy_Implementation()
{
	// 서버에서만 에너미 스폰
	if (HasAuthority())
	{
		// 처음에만 위치값 저장하고 다음부터는 가져다 씀
		FVector loc = GetActorLocation();
		FRotator rot = GetActorRotation();

		FActorSpawnParameters params;
		// 자신을 생성되는 에너미의 오너로 설정
		params.Owner = this;
		params.Instigator = GetInstigator();
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		GetWorld()->SpawnActor<AAlienEnemy>(enemy, loc, rot, params);
	}
}

void AEnemySpawner::PlayerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 부딪힌 대상이 플레이어라면 에너미 스폰
	player = Cast<AAlienSwarmCharacter>(OtherActor);

	if (player)
	{
		if (EnemyCount < MaxEnemyCount) {
			bOverlapToComp = true;
		}
		UE_LOG(LogTemp, Warning, TEXT("Overlaped on Player"));
		// SpawnEnemy();
	}

}

float AEnemySpawner::CalPlayetDist()
{
	float minDist = 100000000.0f;
	for( int32 i = 0; i < playerList.Num(); i++){
		if (playerList[i] != nullptr)
		{
			float tempDist = FVector::Dist(GetActorLocation(), playerList[i]->GetActorLocation());
			if (minDist > tempDist)
			{
				minDist = tempDist;
			}
		}
	}
	return minDist;
}



