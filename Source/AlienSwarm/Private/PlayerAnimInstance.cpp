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


	// 플레이어의 이동속도 가져오기
<<<<<<< HEAD
	auto velociy = player->GetVelocity();
=======
	auto velociy =player->GetVelocity();
>>>>>>> f941cead1e822f0237595fed37505f29f91a73d8

	// 플레이어의 이동 방향과 앞방향을 Dot해서 Vertical에 대입하고싶다.
	Vertical = FVector::DotProduct(velociy, player->GetActorForwardVector());

	// 플레이어의 이동 방향과 앞방향을 Dot해서 Horizontal에 대입하고싶다.
	Horizontal = FVector::DotProduct(velociy, player->GetActorRightVector());

<<<<<<< HEAD
}
=======
}
>>>>>>> f941cead1e822f0237595fed37505f29f91a73d8
