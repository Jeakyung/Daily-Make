// Fill out your copyright notice in the Description page of Project Settings.
// 통신 문제로 사격부분을 전부 플레이어 캐릭터로 이동.
// 발사 간격, 탄환관리만 남아있음.

#include "WeaponRifle.h"
#include "HitInterface.h"
#include "TestPlayerController.h"

bool AWeaponRifle::OnFire(FVector mousePos)
{
	if (bIsFire) {
		return false;
	}

	if (currentAmmo > 0) {
		bIsFire = true;
		/*FVector start = firePoint->GetComponentLocation();
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);
		params.AddIgnoredActor(GetOwner());
		FHitResult hitInfo;
		//Block되는 채널을 찾음
		FVector end = start + firePoint->GetForwardVector() * shootingRange;
		bool bResult = GetWorld()->LineTraceSingleByChannel(hitInfo, start, end, ECC_GameTraceChannel1, params);
		DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 5.0f);
		if (bResult) {
			IHitInterface* target = Cast<IHitInterface>(hitInfo.GetActor());
			if (target) {
				target->TakeHit(damage);
			}
		}*/
		currentAmmo--;
		pc->SetAmmo(currentAmmo);
		return true;
	}
	else {
		return false;
	}
}
