// Fill out your copyright notice in the Description page of Project Settings.


#include "ToolGranade.h"
#include "HitInterface.h"

bool AToolGranade::OnFire(FVector mousePos)
{
	if (bCanFire) {
		bCanFire = false;

		TArray<FOverlapResult> hitsInfos;
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);
		bool bResult = GetWorld()->OverlapMultiByChannel(hitsInfos, mousePos, FQuat::Identity, ECC_GameTraceChannel2, FCollisionShape::MakeSphere(250.0f), params);
		DrawDebugSphere(GetWorld(), mousePos, 250.0f, 32, FColor::Red, false, 3.0f, 0, 2.0f);

		if (bResult) {
			for (FOverlapResult& hit : hitsInfos) {
				IHitInterface* target = Cast<IHitInterface>(hit.GetActor());
				if (target) {
					target->TakeHit(50);
				}
			}
		}

		FTimerHandle reUseTime;
		GetWorld()->GetTimerManager().SetTimer(reUseTime, FTimerDelegate::CreateLambda([&]() {
			bCanFire = true;
			}), coolDown, false);

		return true;
	}

	return false;
}
