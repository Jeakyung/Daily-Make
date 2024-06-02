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
	explosionComp->AddImpulse(FVector(200.f));
	explosionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

}
