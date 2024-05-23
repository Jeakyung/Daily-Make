// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "HitInterface.h"


// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(rootComp);

	gunBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunBody"));
	gunBody->SetupAttachment(rootComp);
	gunBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	firePoint = CreateDefaultSubobject<USceneComponent>(TEXT("FirePoint"));
	firePoint->SetupAttachment(gunBody);
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	currentAmmo = ammo;
	currentMagazine = magazine;
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsFire) {
		currentFireTime += DeltaTime;
		if (currentFireTime > fireRate) {
			bIsFire = false;
			currentFireTime = 0.0f;
		}
	}
	if (bIsReloading) {
		currentReloadTime += DeltaTime;
		if (currentReloadTime > reloadTime) {
			bIsReloading = false;
			currentReloadTime = 0.0f;
			currentAmmo = ammo;
			currentMagazine--;
		}
	}
}

bool AWeaponBase::OnFire(FVector mousePos)
{
	if (bIsFire) {
		return false;
	}

	if (currentAmmo > 0) {
		FVector start = firePoint->GetComponentLocation();
		FVector end = mousePos;
		end.Z = start.Z;
		end = end - start;
		end.Normalize();
		end *= shootingRange;
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);
		params.AddIgnoredActor(GetOwner());
		if (bIsAreaAttack) {
			TArray<FHitResult> hitInfos;
			//Ignore되는 채널을 찾음
			bool bResult = GetWorld()->SweepMultiByChannel(hitInfos, start, end, FQuat::Identity, ECC_GameTraceChannel2, FCollisionShape::MakeSphere(attackArea), params);
			if (bResult) {
				for (FHitResult& hit : hitInfos){
					IHitInterface* target = Cast<IHitInterface>(hit.GetActor());
					if (target) {
						target->TakeHit(damage);
					}
				}
			}
		}
		else {
			FHitResult hitInfo;
			//Block되는 채널을 찾음
			bool bResult = GetWorld()->LineTraceSingleByChannel(hitInfo, start, end, ECC_GameTraceChannel1, params);
			if (bResult) {
				IHitInterface* target = Cast<IHitInterface>(hitInfo.GetActor());
				if (target) {
					target->TakeHit(damage);
				}
			}
		}
		currentAmmo--;
		return true;
	}
	else{
		return false;
	}
}

bool AWeaponBase::OnReload()
{
	if (bIsReloading) {
		return false;
	}

	if (currentMagazine > 0) {
		bIsReloading = true;
		return true;
	}
	else {
		return false;
	}
}

bool AWeaponBase::TakeMagazine()
{
	if (magazine == currentMagazine) {
		return false;
	}
	else {
		currentMagazine++;
		return true;
	}
}

