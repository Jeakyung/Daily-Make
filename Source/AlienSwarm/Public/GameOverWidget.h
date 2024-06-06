
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

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UImage* BackGround;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* GameOver;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* Button_Stay;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* Button_Quit;

	UFUNCTION()
	void OnClickStay();

	UFUNCTION()
	void OnClickQuit();
};
