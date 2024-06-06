// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Button.h"
#include "GameOverWidget.h"
#include "Kismet/KismetSystemLibrary.h"

void UGameOverWidget::NativeConstruct()
{
	Button_Stay->OnClicked.AddDynamic(this, &UGameOverWidget::OnClickStay);
	Button_Quit->OnClicked.AddDynamic(this, &UGameOverWidget::OnClickQuit);
}

void UGameOverWidget::OnClickStay()
{
	// 대기방으로 이동시키기
}

void UGameOverWidget::OnClickQuit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}
