// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HitInterface.h"
#include "BJKTestEnemy.generated.h"

UCLASS()
class ALIENSWARM_API ABJKTestEnemy : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABJKTestEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = Enemy)
	class UStaticMeshComponent* meshComp;

	virtual void TakeHit(int32 damage) override;
};
