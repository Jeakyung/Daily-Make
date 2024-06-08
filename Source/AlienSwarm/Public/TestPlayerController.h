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
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UTitleWidget> titleWidget_bp;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UShopWidget> shopWidget_bp;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UMainWidget> mainWidget_bp;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UGameOverWidget> gameOver_bp;

public:
	UPROPERTY()
	class UTitleWidget* titleWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UShopWidget* shopWidget;

	UPROPERTY()
	class UMainWidget* mainWidget;

	UPROPERTY()
	class UGameOverWidget* GameOverWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UTexture2D* Weapon1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UTexture2D* Weapon2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UTexture2D* SubWeapon;

	void MakeTitleWidget();

	void MakeShopWidget();

	void MakeMainWidget();

	void MakeGameOverWidget();

	void MoveToStartPos();

	UFUNCTION(Server, Reliable)
	void ServerRPC_MovePos();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_MovePos(FVector MovePos);

	void SetHP(float value);

	void SetAmmo(int32 value);

	void SetMeg(int32 value);

	// 관전자 모드 (일단 안씀 Enemy와 충돌남)
	UFUNCTION(Server, Reliable)
	void ServerRPC_ChangeSpectator();

	class AMainGameModeBase* gameMode;
};
