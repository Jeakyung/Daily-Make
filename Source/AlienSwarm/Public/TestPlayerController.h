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

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UShopWidget> shopWidget_bp;

	UPROPERTY()
	class UShopWidget* shopWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UMainWidget> mainWidget_bp;

	UPROPERTY()
	class UMainWidget* mainWidget;

public:
	void MakeTitleWidget();

	void MakeShopWidget();

	void MakeMainWidget();

	void SetHP(float value);

	void SetAmmo(int32 value);

	void SetMeg(int32 value);
};
