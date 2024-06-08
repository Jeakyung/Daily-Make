// Fill out your copyright notice in the Description page of Project Settings.

#include "GameOverWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/CanvasPanel.h>
#include "TestPlayerController.h"


void UGameOverWidget::NativeConstruct()
{
	Button_Stay->OnClicked.AddDynamic(this, &UGameOverWidget::OnClickStay);
	Button_Quit->OnClicked.AddDynamic(this, &UGameOverWidget::OnClickQuit);
	Button_ShowOP->OnClicked.AddDynamic(this, &UGameOverWidget::OnClickShowOP);
}



void UGameOverWidget::OnClickStay()
{
	// 대기방으로 이동시키기

	

}

void UGameOverWidget::OnClickQuit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void UGameOverWidget::OnClickShowOP()
{
	// 관전 모드 실행
	PlayerController->ServerRPC_ChangeSpectator(true);
}
