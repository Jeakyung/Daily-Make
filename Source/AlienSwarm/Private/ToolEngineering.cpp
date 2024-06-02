// Fill out your copyright notice in the Description page of Project Settings.


#include "ToolEngineering.h"
#include "NiagaraComponent.h"

AToolEngineering::AToolEngineering()
{
	aimmingLaser->bAutoActivate = false;
}

bool AToolEngineering::OnFire(FVector mousePos)
{
	//아무것도 안함
	return false;
}
