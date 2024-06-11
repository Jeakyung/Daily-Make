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
//		// �ֺ��� ANetTPSGSCharacter�� ��� �˻��ϰ�ʹ�.
//		for (TActorIterator<AAlienSwarmCharacter> it(GetWorld()); it; ++it)
//		{
//			AActor* otherActor = *it;
//			// ������ �Ÿ��� �缭 
//			float tempDist = otherActor->GetDistanceTo(this);
//			// ���� minDist���� �����ٸ� ����ϰ�
//			if (tempDist < minDist)
//			{
//				// ���� ����� ANetTPSGSCharacter�� newOwner�� ����ؼ�
//				minDist = tempDist;
//				newOwner = otherActor;
//			}
//		}
//		// ���� ���� �����ʰ� newOwner�� �ٸ��ٸ�
//		if (GetOwner() != newOwner) {
//			// ���� ���ʷ� �ϰ�ʹ�.
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