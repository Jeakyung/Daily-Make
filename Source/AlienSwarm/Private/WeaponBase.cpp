// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "HitInterface.h"
#include "../AlienSwarmCharacter.h"
#include "TestPlayerController.h"


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

	aimmingLaser = CreateDefaultSubobject<UNiagaraComponent>(TEXT("AimmingLaser"));
	aimmingLaser->SetupAttachment(firePoint);
	

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
}

bool AWeaponBase::OnFire(FVector mousePos)
{
	return false;
}

bool AWeaponBase::OnReload()
{
	if (currentMagazine > 0) {
		currentAmmo = ammo;
		currentMagazine--;
		pc->SetAmmo(currentAmmo);
		pc->SetMeg(currentMagazine);
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
		pc->SetMeg(currentMagazine);
		return true;
	}
}

void AWeaponBase::CalculateEndPoint(FVector mousePos)
{
	FVector start = firePoint->GetComponentLocation();
	end = mousePos;
	end.Z += start.Z;
	end = end - start;
	end.Normalize();
	end *= shootingRange;
	end += start;
}

void AWeaponBase::Equip(AActor* ownedActor)
{
	playerREF = Cast<AAlienSwarmCharacter>(ownedActor);
	if (playerREF) {
		SetOwner(playerREF);
		pc = Cast<ATestPlayerController>(playerREF->GetOwner());
	}

	if (pc) {
		pc->SetAmmo(currentAmmo);
		pc->SetMeg(currentMagazine);
	}
}

void AWeaponBase::UnEquip()
{
	//무기 교체시 사용될 예정? 맵 구석으로 이동시키고 안보임 처리하는게 좋은거 아닌지?
	pc = nullptr;
	playerREF = nullptr;
	SetOwner(nullptr);
}

