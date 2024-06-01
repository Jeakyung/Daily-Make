// Fill out your copyright notice in the Description page of Project Settings.


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
	end.Z = start.Z;

	FHitResult hits;
	bool bHit = GetWorld()->LineTraceSingleByChannel(hits, start, end, ECC_Visibility);
	if (bHit) {
		end = hits.ImpactPoint;
	}

	ServerRPC_CalEndPoint(end);
}

void AWeaponBase::ServerRPC_CalEndPoint_Implementation(FVector _end)
{
	end = _end;

	//UE_LOG(LogTemp, Warning, TEXT("CalendPos : %f %f %f"), end.X, end.Y, end.Z);

	OnRep_CalEndPoint();
	//MultiRPC_CalEndPoint(end);
	//ClientRPC_CalEndPoint(end);
}

void AWeaponBase::ClientRPC_CalEndPoint_Implementation(FVector _endPos)
{
	//UE_LOG(LogTemp, Warning, TEXT("ClientendPos : %f %f %f"), _endPos.X, _endPos.Y, _endPos.Z);
	aimmingLaser->SetVectorParameter(TEXT("Beam End"), _endPos);
}

void AWeaponBase::MultiRPC_CalEndPoint_Implementation(FVector _endPos)
{
	//UE_LOG(LogTemp, Warning, TEXT("MultiendPos : %f %f %f"), _endPos.X, _endPos.Y, _endPos.Z);
	aimmingLaser->SetVectorParameter(TEXT("Beam End"), _endPos);
}

void AWeaponBase::OnRep_CalEndPoint()
{
	//UE_LOG(LogTemp, Warning, TEXT("OnRependPos : %f %f %f"), _endPos.X, _endPos.Y, _endPos.Z);
	aimmingLaser->SetVectorParameter(TEXT("Beam End"), end);
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

void AWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//rotYaw를 일정 주기마다 각 클라이언트에 뿌려서 클라이언트의 변수값을 고찬다,
	DOREPLIFETIME(AWeaponBase, end);
}