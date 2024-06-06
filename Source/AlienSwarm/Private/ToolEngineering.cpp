// Fill out your copyright notice in the Description page of Project Settings.


#include "ToolEngineering.h"
#include "NiagaraComponent.h"
#include "DoorActor.h"

AToolEngineering::AToolEngineering()
{
	aimmingLaser->bAutoActivate = false;
}

bool AToolEngineering::OnFire(FVector mousePos)
{
	if (!bCanFire) {
		return false;
	}
	else {
		SetOwner(GetWorld()->GetFirstPlayerController()->GetPawn());
		bCanFire = false;

		TArray<FOverlapResult> hitsInfos;
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);
		bool bResult = GetWorld()->OverlapMultiByChannel(hitsInfos, GetActorLocation(), FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(150.0f), params);
		DrawDebugSphere(GetWorld(), GetActorLocation(), 150.0f, 32, FColor::Blue, false, 3.0f, 0, 2.0f);

		if (bResult) {
			for (FOverlapResult& hit : hitsInfos) {
				ADoorActor* target = Cast<ADoorActor>(hit.GetActor());
				if (target) {
					UE_LOG(LogTemp, Warning, TEXT("EngTool : Find Door"));
					if (!target->bIsLocked) {
						target->DoorLock();
						if (target->bIsOpened) {
							target->ServerRPC_DoorClose();
						}
					}
					else {
						target->DoorUnLock();
						if (!target->bIsOpened) {
							target->ServerRPC_DoorOpen();
						}
					}
					break;
				}
			}
		}

		FTimerHandle reUseTime;
		GetWorld()->GetTimerManager().SetTimer(reUseTime, FTimerDelegate::CreateLambda([&]() {
			bCanFire = true;
			}), coolDown, false);
	}
	//아무것도 안함
	return true;
}
