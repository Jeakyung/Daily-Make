// Fill out your copyright notice in the Description page of Project Settings.


#include "ToolBulletBox.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "../AlienSwarmCharacter.h"
#include "EngineUtils.h"

AToolBulletBox::AToolBulletBox()
{
	PrimaryActorTick.bCanEverTick = true;

	cheakArea = CreateDefaultSubobject<UBoxComponent>(TEXT("cheakArea"));
	cheakArea->SetupAttachment(gunBody);
	cheakArea->SetBoxExtent(FVector(100.0f));

	aimmingLaser->bAutoActivate = false;
}

void AToolBulletBox::BeginPlay()
{
	Super::BeginPlay();

	cheakArea->OnComponentBeginOverlap.AddDynamic(this, &AToolBulletBox::ChargeMeg);
}

void AToolBulletBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//CheckOwner();
}

//void AToolBulletBox::CheckOwner()
//{
//	if (HasAuthority())
//	{
//		float minDist = 100.0f;
//		AActor* newOwner = nullptr;
//		// 주변의 ANetTPSGSCharacter을 계속 검색하고싶다.
//		for (TActorIterator<AAlienSwarmCharacter> it(GetWorld()); it; ++it)
//		{
//			AActor* otherActor = *it;
//			// 나와의 거리를 재서 
//			float tempDist = otherActor->GetDistanceTo(this);
//			// 만약 minDist보다 가깝다면 기억하고
//			if (tempDist < minDist)
//			{
//				// 가장 가까운 ANetTPSGSCharacter를 newOwner로 기억해서
//				minDist = tempDist;
//				newOwner = otherActor;
//			}
//		}
//		// 만약 현재 내오너가 newOwner와 다르다면
//		if (GetOwner() != newOwner) {
//			// 나의 오너로 하고싶다.
//			SetOwner(newOwner);
//		}
//	}
//}

bool AToolBulletBox::OnFire(FVector mousePos)
{
	if (bCanFire) {
		SetOwner(GetWorld()->GetFirstPlayerController()->GetPawn());
		bCanFire = false;

		/*FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AToolBulletBox* setBox = GetWorld()->SpawnActor<AToolBulletBox>(bulletBox_BP, GetActorLocation() + FVector(0.0f, 0.0f, -125.0f), FRotator::ZeroRotator, params);
		setBox->bSet = true;*/

		FTimerHandle reUseTime;
		GetWorld()->GetTimerManager().SetTimer(reUseTime, FTimerDelegate::CreateLambda([&]() {
			bCanFire = true;
			}), coolDown, false);
		return true;
	}

	return false;
}

void AToolBulletBox::ChargeMeg(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AAlienSwarmCharacter* touchedPlayer = Cast<AAlienSwarmCharacter>(OtherActor);
	if (bSet && touchedPlayer && HasAuthority()) {
		SetOwner(touchedPlayer->GetController());
		ServerRPC_ReduceMeg(touchedPlayer);
	}
}

void AToolBulletBox::ServerRPC_ReduceMeg_Implementation(AAlienSwarmCharacter* touchedPlayer)
{
	MultiRPC_ReduceMeg(touchedPlayer, megCount);
	//SetOwner(nullptr);
}

void AToolBulletBox::MultiRPC_ReduceMeg_Implementation(AAlienSwarmCharacter* touchedPlayer, int32 _meg)
{
	bool bIsTake;
	switch (touchedPlayer->SelectedWeapon)
	{
	case 1:
		if (touchedPlayer->Weapon != nullptr) {
			bIsTake = touchedPlayer->Weapon->TakeMagazine();
		}
		else {
			bIsTake = false;
		}
		break;
	case 2:
		if (touchedPlayer->Weapon2 != nullptr) {
			bIsTake = touchedPlayer->Weapon2->TakeMagazine();
		}
		else {
			bIsTake = false;
		}
		break;
	default:
		bIsTake = false;
		break;
	}

	if (bIsTake) {
		megCount--;
	}
	if (megCount == 0) {
		//if (HasAuthority())
		{
			ServerRPC_Destroy();
		}
	}
}

void AToolBulletBox::ServerRPC_Destroy_Implementation()
{
	Destroy();
}