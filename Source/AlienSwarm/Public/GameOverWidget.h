#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"


UCLASS()
class ALIENSWARM_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;


	// Game Over UI
	// Panel 연결
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UCanvasPanel* GameOverPanel;
	
	// 뒷배경 이미지 연결
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UImage* BackGround;


	// 게임오버 글씨 연결
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* GameOver;


	// 대기방에 머물기 버튼 연결
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* Button_Stay;

	// 게임 나가기 버튼 연결
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* Button_Quit;

	// 관전모드 버튼 연결
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* Button_ShowOP;


	class ATestPlayerController* PlayerController;




	UFUNCTION()
	void OnClickStay();

	UFUNCTION()
	void OnClickQuit();

	UFUNCTION()
	void OnClickShowOP();


};
