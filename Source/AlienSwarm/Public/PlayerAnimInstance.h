


#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"


UCLASS()
class ALIENSWARM_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UPROPERTY()
	class AAlienSwarmCharacter* player;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Horizontal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Vertical;

	
	
};
