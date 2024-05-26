// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleWidget.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSWARM_API UTitleWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* Btn_Create;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* Btn_Join;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* Btn_Quit;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UVerticalBox* RoomList;

	UFUNCTION()
	void CreateRoom();

	UFUNCTION()
	void ShowRoomList();

	UFUNCTION()
	void QuitGame();
};
