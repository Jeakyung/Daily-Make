// Fill out your copyright notice in the Description page of Project Settings.



#include "PlayerAnimInstance.h"
#include "../AlienSwarmCharacter.h"


void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	player = Cast<AAlienSwarmCharacter>(TryGetPawnOwner());
}






void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (nullptr == player)
	{
		return;
	}


	// �÷��̾��� �̵��ӵ� ��������

	auto velociy = player->GetVelocity();



	// �÷��̾��� �̵� ����� �չ����� Dot�ؼ� Vertical�� �����ϰ�ʹ�.
	Vertical = FVector::DotProduct(velociy, player->GetActorForwardVector());

	// �÷��̾��� �̵� ����� �չ����� Dot�ؼ� Horizontal�� �����ϰ�ʹ�.
	Horizontal = FVector::DotProduct(velociy, player->GetActorRightVector());


}



