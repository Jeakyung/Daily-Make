// Fill out your copyright notice in the Description page of Project Settings.


<<<<<<< HEAD
#include "PlayerAnimInstance.h"
#include "../AlienSwarmCharacter.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
=======
#include "MyAnimInstance.h"
#include "../AlienSwarmCharacter.h"

void UMyAnimInstance::NativeInitializeAnimation()
>>>>>>> f941cead1e822f0237595fed37505f29f91a73d8
{
	Super::NativeInitializeAnimation();
	player = Cast<AAlienSwarmCharacter>(TryGetPawnOwner());
}

<<<<<<< HEAD

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
=======
void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
>>>>>>> f941cead1e822f0237595fed37505f29f91a73d8
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (nullptr == player)
	{
		return;
	}


	// �÷��̾��� �̵��ӵ� ��������
<<<<<<< HEAD
	auto velociy = player->GetVelocity();
=======
	auto velociy =player->GetVelocity();
>>>>>>> f941cead1e822f0237595fed37505f29f91a73d8

	// �÷��̾��� �̵� ����� �չ����� Dot�ؼ� Vertical�� �����ϰ�ʹ�.
	Vertical = FVector::DotProduct(velociy, player->GetActorForwardVector());

	// �÷��̾��� �̵� ����� �չ����� Dot�ؼ� Horizontal�� �����ϰ�ʹ�.
	Horizontal = FVector::DotProduct(velociy, player->GetActorRightVector());

<<<<<<< HEAD
}
=======
}
>>>>>>> f941cead1e822f0237595fed37505f29f91a73d8
