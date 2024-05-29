// Fill out your copyright notice in the Description page of Project Settings.


#include "BJKTestEnemy.h"
#include "HitInterface.h"

// Sets default values
ABJKTestEnemy::ABJKTestEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
}

// Called when the game starts or when spawned
void ABJKTestEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABJKTestEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABJKTestEnemy::TakeHit(int32 damage)
{
	UE_LOG(LogTemp, Warning, TEXT("TakeDamage"));
}
