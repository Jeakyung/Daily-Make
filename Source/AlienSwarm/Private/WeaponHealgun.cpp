// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponHealgun.h"
#include "HitInterface.h"
#include "TestPlayerController.h"

bool AWeaponHealgun::OnFire(FVector mousePos)
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
		FHitResult hitInfo;
		//Block되는 채널을 찾음
		bool bResult = GetWorld()->LineTraceSingleByChannel(hitInfo, start, end, ECC_GameTraceChannel1, params);
		DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 5.0f);
		if (bResult) {
			IHitInterface* target = Cast<IHitInterface>(hitInfo.GetActor());
			if (target) {
				target->TakeHit(damage);
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
