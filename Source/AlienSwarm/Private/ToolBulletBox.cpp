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
		ServerRPC_SetBox(mousePos);
		return true;
	}

	return false;
}

void AToolBulletBox::ServerRPC_SetBox_Implementation(FVector mousePos)
{
	/*bCanFire = false;
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AToolBulletBox* setBox = GetWorld()->SpawnActor<AToolBulletBox>(bulletBox_BP, GetActorLocation() + FVector(0.0f, 0.0f, -125.0f), FRotator::ZeroRotator, params);
	setBox->bSet = true;

	FTimerHandle reUseTime;
	GetWorld()->GetTimerManager().SetTimer(reUseTime, FTimerDelegate::CreateLambda([&]() {
		bCanFire = true;
		}), coolDown, false);*/

	MultiRPC_SetBox(mousePos);
}

void AToolBulletBox::MultiRPC_SetBox_Implementation(FVector mousePos)
{
	bCanFire = false;
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AToolBulletBox* setBox = GetWorld()->SpawnActor<AToolBulletBox>(bulletBox_BP, GetActorLocation() + FVector(0.0f, 0.0f, -125.0f), FRotator::ZeroRotator, params);
	setBox->bSet = true;

	FTimerHandle reUseTime;
	GetWorld()->GetTimerManager().SetTimer(reUseTime, FTimerDelegate::CreateLambda([&]() {
		bCanFire = true;
		}), coolDown, false);
}

void AToolBulletBox::ChargeMeg(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ServerRPC_ReduceMeg(OtherActor);
}

void AToolBulletBox::ServerRPC_ReduceMeg_Implementation(AActor* OtherActor)
{
	if (bSet) {
		AAlienSwarmCharacter* touchedPlayer = Cast<AAlienSwarmCharacter>(OtherActor);
		if (touchedPlayer) {
			//SetOwner(touchedPlayer);
			bool bIsTake;
			switch (touchedPlayer->SelectedWeapon)
			{
			case 1:
				bIsTake = touchedPlayer->Weapon->TakeMagazine();
				break;
			case 2:
				bIsTake = touchedPlayer->Weapon2->TakeMagazine();
				break;
			default:
				bIsTake = false;
				break;
			}

			if (bIsTake) {
				megCount--;
				MultiRPC_ReduceMeg(megCount);
			}
		}
	}
	//SetOwner(nullptr);
}

void AToolBulletBox::MultiRPC_ReduceMeg_Implementation(int32 _meg)
{
	megCount = _meg;
	if (megCount == 0) {
		Destroy();
	}
}
