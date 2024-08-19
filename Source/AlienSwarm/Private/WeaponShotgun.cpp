// Fill out your copyright notice in the Description page of Project Settings.
// 통신 문제로 사격부분을 전부 플레이어 캐릭터로 이동.
// 발사 간격, 탄환관리만 남아있음.

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
		/*FVector start = firePoint->GetComponentLocation();
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);
		params.AddIgnoredActor(GetOwner());
		TArray<FHitResult> hitInfos;
		//Ignore되는 채널을 찾음
		FVector end = start + firePoint->GetForwardVector() * shootingRange;
		UE_LOG(LogTemp, Warning, TEXT("Z = %f"), start.Z);
		end.Z = start.Z - 150.0f;
		bool bResult = GetWorld()->SweepMultiByChannel(hitInfos, start, end, FQuat::Identity, ECC_GameTraceChannel2, FCollisionShape::MakeSphere(attackArea), params);
		DrawDebugCylinder(GetWorld(), start, end, attackArea, 32, FColor::Red, false, 5.0f);
		if (bResult) {
			for (FHitResult& hit : hitInfos) {
				IHitInterface* target = Cast<IHitInterface>(hit.GetActor());
				if (target) {
					target->TakeHit(damage);
				}
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
