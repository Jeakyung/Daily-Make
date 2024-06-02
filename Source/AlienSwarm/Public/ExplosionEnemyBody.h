// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeometryCollection/GeometryCollectionActor.h"
#include "ExplosionEnemyBody.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSWARM_API AExplosionEnemyBody : public AGeometryCollectionActor
{
	GENERATED_BODY()

public:
	AExplosionEnemyBody();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Comp")
	class UGeometryCollectionComponent* explosionComp;

protected:
	virtual void BeginPlay() override;

	
};
