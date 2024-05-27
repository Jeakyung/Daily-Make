// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponShotgun.h"
#include "HitInterface.h"
#include "TestPlayerController.h"

bool AWeaponShotgun::OnFire(FVector mousePos)
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
		TArray<FHitResult> hitInfos;
		//Ignore되는 채널을 찾음
		bool bResult = GetWorld()->SweepMultiByChannel(hitInfos, start, end, FQuat::Identity, ECC_GameTraceChannel2, FCollisionShape::MakeSphere(attackArea), params);
		DrawDebugCylinder(GetWorld(), start, end, attackArea, 32, FColor::Red, false, 5.0f);
		if (bResult) {
			for (FHitResult& hit : hitInfos) {
				IHitInterface* target = Cast<IHitInterface>(hit.GetActor());
				if (target) {
					target->TakeHit(damage);
				}
			}
		}
		currentAmmo--;
		pc->SetAmmo(currentAmmo);
		return true;
	}
	else {
		return false;
	}
}
