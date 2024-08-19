// Fill out your copyright notice in the Description page of Project Settings.
// 무기, 장비류 인스턴스들의 기본이 되는 클래스.
// 탄환 발사, 재장전, 탄창 획득, 장비의 4가지 기능을 사용하게 한다.

#include "WeaponBase.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "HitInterface.h"
#include "../AlienSwarmCharacter.h"
#include "TestPlayerController.h"
#include "Net/UnrealNetwork.h"


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
	firePoint->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

	aimmingLaser = CreateDefaultSubobject<UNiagaraComponent>(TEXT("AimmingLaser"));
	aimmingLaser->SetupAttachment(firePoint);
	
	bReplicates = true;
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
		if (pc) {
			pc->SetAmmo(currentAmmo);
			pc->SetMeg(currentMagazine);
		}
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