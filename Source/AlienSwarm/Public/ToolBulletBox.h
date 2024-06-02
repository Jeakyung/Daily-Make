// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "ToolBulletBox.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSWARM_API AToolBulletBox : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	AToolBulletBox();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* cheakArea;

	bool bCanFire = true;
	float coolDown = 5.0f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AToolBulletBox> bulletBox_BP;

	UPROPERTY(EditAnywhere)
	bool bSet = false;

	//UPROPERTY(Replicated)
	int32 megCount = 1;

	//void CheckOwner();

	virtual bool OnFire(FVector mousePos) override;

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetBox(FVector mousePos);

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_SetBox(FVector mousePos);

	UFUNCTION()
	void ChargeMeg(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Server, Reliable)
	void ServerRPC_ReduceMeg(AActor* OtherActor);

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_ReduceMeg(int32 _meg);
};
