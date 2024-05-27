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
	firePoint->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

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
	if (bIsFire) {
		return false;
	}

	if (currentAmmo > 0) {
		bIsFire = true;
		FVector start = firePoint->GetComponentLocation();
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);
		params.AddIgnoredActor(GetOwner());
		if (bIsAreaAttack) {
			TArray<FHitResult> hitInfos;
			//Ignore�Ǵ� ä���� ã��
			bool bResult = GetWorld()->SweepMultiByChannel(hitInfos, start, end, FQuat::Identity, ECC_GameTraceChannel2, FCollisionShape::MakeSphere(attackArea), params);
			DrawDebugCylinder(GetWorld(), start, end, attackArea, 32, FColor::Red, false, 5.0f);
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
			//Block�Ǵ� ä���� ã��
			bool bResult = GetWorld()->LineTraceSingleByChannel(hitInfo, start, end, ECC_GameTraceChannel1, params);
			DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 5.0f);
			if (bResult) {
				IHitInterface* target = Cast<IHitInterface>(hitInfo.GetActor());
				if (target) {
					target->TakeHit(damage);
				}
			}
		}
		currentAmmo--;
		pc->SetAmmo(currentAmmo);
		return true;
	}
	else{
		return false;
	}
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
	//���� ��ü�� ���� ����? �� �������� �̵���Ű�� �Ⱥ��� ó���ϴ°� ������ �ƴ���?
	pc = nullptr;
	playerREF = nullptr;
	SetOwner(nullptr);
}

