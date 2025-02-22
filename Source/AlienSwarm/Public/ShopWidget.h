// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopWidget.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSWARM_API UShopWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UButton* Btn_Start;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void StartGame();

	// 무기를 스폰한다.
	UFUNCTION(Server, Reliable)
	void ServerRPC_Test();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_Test();
};
