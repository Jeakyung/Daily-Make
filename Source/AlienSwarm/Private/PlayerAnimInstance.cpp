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


	// 플레이어의 이동속도 가져오기

	auto velociy = player->GetVelocity();



	// 플레이어의 이동 방향과 앞방향을 Dot해서 Vertical에 대입하고싶다.
	Vertical = FVector::DotProduct(velociy, player->GetActorForwardVector());

	// 플레이어의 이동 방향과 앞방향을 Dot해서 Horizontal에 대입하고싶다.
	Horizontal = FVector::DotProduct(velociy, player->GetActorRightVector());


}



