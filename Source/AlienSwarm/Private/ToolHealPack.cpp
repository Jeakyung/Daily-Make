// Fill out your copyright notice in the Description page of Project Settings.


#include "ToolHealPack.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "../AlienSwarmCharacter.h"

AToolHealPack::AToolHealPack()
{
	PrimaryActorTick.bCanEverTick = true;

	aimmingLaser->bAutoActivate = false;
}

void AToolHealPack::BeginPlay()
{
	Super::BeginPlay();
}

void AToolHealPack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bSet) {
		HealCheak();
	}
}

bool AToolHealPack::OnFire(FVector mousePos)
{
	if (bCanFire) {
		bCanFire = false;
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AToolHealPack* setBox = GetWorld()->SpawnActor<AToolHealPack>(healPack_BP, GetActorLocation() + FVector(0.0f, 0.0f, -125.0f), FRotator::ZeroRotator, params);
		setBox->bSet = true;

		FTimerHandle reUseTime;
		GetWorld()->GetTimerManager().SetTimer(reUseTime, FTimerDelegate::CreateLambda([&]() {
			bCanFire = true;
			}), coolDown, false);

		return true;
	}

	return false;
}

void AToolHealPack::HealCheak()
{
	currentTime += GetWorld()->GetDeltaSeconds();
	if (currentTime > healedTime) {
		healedTime += 1.0f;
		TArray<FOverlapResult> hitsInfos;
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);
		bool bResult = GetWorld()->OverlapMultiByChannel(hitsInfos, GetActorLocation(), FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(250.0f), params);
		DrawDebugSphere(GetWorld(), GetActorLocation(), 250.0f, 32, FColor::Green, false, 3.0f, 0, 2.0f);

		if (bResult) {
			for (FOverlapResult& hit : hitsInfos) {
				AAlienSwarmCharacter* target = Cast<AAlienSwarmCharacter>(hit.GetActor());
				if (target) {
					target->TakeHit(-200);
				}
			}
		}
	}

	if (currentTime > healDuration) {
		Destroy();
	}
}
