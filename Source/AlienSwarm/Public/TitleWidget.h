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
	class UButton* Btn_CloseRoomSetting;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* Btn_InsideCreate;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* Btn_Join;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* Btn_Quit;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* Btn_CloseRoomList;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* Btn_Refresh;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UScrollBox* RoomList;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UEditableText* EditedRoomName;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* RoomListSpan;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* RoomSettingSpan;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UCircularThrobber* Throbber_Loading;

	UPROPERTY(EditDefaultsOnly)
	class UAlienSwarmGameInstance* gi;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class URoomListItemUI> roomItemUIFactory;

	UFUNCTION()
	void ShowRoomSetting();

	UFUNCTION()
	void CloseRoomSetting();

	UFUNCTION()
	void CreateRoom();

	UFUNCTION()
	void ShowRoomList();

	UFUNCTION()
	void CloseRoomList();

	UFUNCTION()
	void QuitGame();

	UFUNCTION()
	void OnClickFindRoom();

	UFUNCTION()
	void OnMySetActiveFindingThrobber(bool bSearching);

	UFUNCTION()
	void AddRoomInfoUI(const struct FSessionInfo& info);
};
