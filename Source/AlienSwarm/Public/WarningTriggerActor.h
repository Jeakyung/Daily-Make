// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WarningTriggerActor.generated.h"

UCLASS()
class ALIENSWARM_API AWarningTriggerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWarningTriggerActor();

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* rootComp;
	
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* triggerComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	AActor* dirLight;

	UFUNCTION()
	void ActiveWarning(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Server, Reliable)
	void ServerRPC_ActiveWarning();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_ActiveWarning(const TArray<class ARedLightActor*>& redLights, const TArray<class ADoorActor*>& doors, const TArray<AEnemySpawner*>& spawners);
};
