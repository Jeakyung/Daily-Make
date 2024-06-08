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
	// Panel ����
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UCanvasPanel* GameOverPanel;
	
	// �޹�� �̹��� ����
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UImage* BackGround;


	// ���ӿ��� �۾� ����
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* GameOver;


	// ���濡 �ӹ��� ��ư ����
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* Button_Stay;

	// ���� ������ ��ư ����
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* Button_Quit;

	// ������� ��ư ����
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
