// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UCLASS()
class ALIENSWARM_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* rootComp;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* gunBody;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USceneComponent* firePoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UNiagaraComponent* aimmingLaser;

private:
	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	int32 damage;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	float fireRate;
	float currentFireTime = 0.0f;
	bool bIsFire = false;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	float reloadTime;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	int32 ammo;
	int32 currentAmmo;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	int32 magazine;
	int32 currentMagazine;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	float shootingRange;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	bool bIsAreaAttack;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	float attackArea;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	int32 cost;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	class UTexture2D* thumbnailImg;

	UPROPERTY()
	class AAlienSwarmCharacter* playerREF;

	UPROPERTY()
	class ATestPlayerController* pc;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector end;

	UFUNCTION()
	bool OnFire(FVector mousePos);

	UFUNCTION()
	bool OnReload();
	
	UFUNCTION()
	bool TakeMagazine();

	UFUNCTION()
	void CalculateEndPoint(FVector mousePos);

	UFUNCTION()
	FORCEINLINE int32 GetCurrentAmmo() {return currentAmmo;}

	UFUNCTION()
	FORCEINLINE int32 GetCurrentMagazine() {return currentMagazine;}

	UFUNCTION()
	FORCEINLINE int32 GetCost() {return cost;}

	UFUNCTION()
	void Equip(AActor* ownedActor);

	UFUNCTION()
	void UnEquip();
};
