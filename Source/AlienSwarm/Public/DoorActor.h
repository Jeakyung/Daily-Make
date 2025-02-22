// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HitInterface.h"
#include "AlienEnemy.h"
#include "DoorActor.generated.h"

UCLASS()
class ALIENSWARM_API ADoorActor : public AActor, public IHitInterface
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

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* lockBox;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(ReplicatedUsing = OnRep_bIsLocked, EditAnywhere)
	bool bIsLocked = false;

	UFUNCTION()
	void OnRep_bIsLocked();

	UPROPERTY(EditAnywhere, Category=door)
	bool bIsOpened = false;

	bool openstart = false;

	bool closestart = false;

	float doorTimer = 0.0f;

	float doorOpenTime = 0.5f;

	UPROPERTY(ReplicatedUsing=OnRep_HPCheck, VisibleAnywhere)
	int32 doorHP = 600;

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

	void DoorLock();

	UFUNCTION(Server, Reliable)
	void ServerRPC_DoorLock();

	void DoorUnLock();

	UFUNCTION(Server, Reliable)
	void ServerRPC_DoorUnLock();

	UFUNCTION()
	void OnRep_HPCheck();

	virtual void TakeHit(int32 damage) override;
	
	UPROPERTY(EditAnywhere, Category=door)
	bool bIsDestroyed = false;

	//UFUNCTION()
	/*void EnemyOverlapDoorClosed(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	*/

	TArray<AAlienEnemy*> enemyList;
};
