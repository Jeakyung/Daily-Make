// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosionEnemyBody.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

AExplosionEnemyBody::AExplosionEnemyBody()
{
	explosionComp = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("ExplosionComp"));
	explosionComp->SetupAttachment(RootComponent);
}

void AExplosionEnemyBody::BeginPlay()
{
	explosionComp->AddImpulse(FVector(350.f));
	explosionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	explosionComp->DamageThreshold[0] = 1.f;
	explosionComp->DamageThreshold[1] = 1.f;
	explosionComp->DamageThreshold[2] = 1.f;

	// use root proxy for navigation => true 
}
