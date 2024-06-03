// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class ALIENSWARM_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AAlienEnemy> enemy;

	UPROPERTY(EditAnywhere)
	class AAlienSwarmCharacter* player;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SpawnEnemy();

	UPROPERTY(EditAnywhere)
	class USceneComponent* sceneComp;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* overlapCheckBox;

	UFUNCTION()
	void PlayerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// ���ʹ� �����Ǵ� �ð� ����
	float currentTime=0.f;
	float SpawnTime = 2.f;

	// �����ʴ� �ִ� ���� ���ʹ� ����
	int MaxEnemyCount = 5;
	int EnemyCount = 0;

	// �������Ǹ� ���ʹ� ����
	bool bOverlapToComp = false;

	// ���� ����
	bool bSpawnInfinity = false;


};
