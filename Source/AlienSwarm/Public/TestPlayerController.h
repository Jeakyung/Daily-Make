// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TestPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSWARM_API ATestPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	// To add mapping context
	virtual void BeginPlay();

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UTitleWidget> titleWidget_bp;

	UPROPERTY()
	class UTitleWidget* titleWidget;


};
