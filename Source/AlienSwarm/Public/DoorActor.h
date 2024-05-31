// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorActor.generated.h"

UCLASS()
class ALIENSWARM_API ADoorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorActor();

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* rootComp;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* doorFrame;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* door;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* openColl;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool bIsLocked = false;

	bool bIsOpened = false;

	bool openstart = false;

	bool closestart = false;

	float doorTimer = 0.0f;

	float doorOpenTime = 0.5f;

	FVector doorClosePos;

	FVector doorOpenPos;

	UPROPERTY(Replicated)
	FVector doorPos;
	
	/*UPROPERTY(Replicated)
	void EnemyAttackDoor();*/

	UFUNCTION()
	void DoorOpen(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Server, Reliable)
	void ServerRPC_DoorOpen();

	UFUNCTION()
	void DoorClose(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(Server, Reliable)
	void ServerRPC_DoorClose();
};
