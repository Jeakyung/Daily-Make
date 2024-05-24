// Fill out your copyright notice in the Description page of Project Settings.


#include "TestPlayerController.h"
#include "TitleWidget.h"

void ATestPlayerController::BeginPlay()
{
	//���� �����ִ� ���� �̸��� TitleLevel�̸�(���� ���ϵǴ� ���� UEDPIE_0_TitleLevel��)
	if (GetWorld()->GetMapName().Contains(TEXT("TitleLevel"))) {
		//Ÿ��Ʋ ������ ��������
		if (titleWidget_bp) {
			titleWidget = CreateWidget<UTitleWidget>(GetWorld(), titleWidget_bp);
			if (titleWidget) {
				titleWidget->AddToViewport();
			}
		}
	}
}
