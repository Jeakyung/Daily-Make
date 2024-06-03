// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RedLightActor.generated.h"

UCLASS()
class ALIENSWARM_API ARedLightActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARedLightActor();

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* rootComp;

	UPROPERTY(EditDefaultsOnly)
	class USpotLightComponent* redLight1;

	UPROPERTY(EditDefaultsOnly)
	class USpotLightComponent* redLight2;

	UPROPERTY(EditDefaultsOnly)
	class URotatingMovementComponent* rotateComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Server, Reliable)
	void ServerRPC_LightOn();

	UFUNCTION(NetMulticast, Reliable)
	void MulitRPC_LightOn();
};
