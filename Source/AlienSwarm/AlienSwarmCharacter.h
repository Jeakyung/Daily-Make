// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "AlienSwarmCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AAlienSwarmCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;


	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IA_Fire;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IA_Reload;

public:
	AAlienSwarmCharacter();
	
	virtual void Tick(float DeltaTime) override;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	// 발사하는 기능
	void OnIAFire(const FInputActionValue& Value);
	
	// 재장전하는 기능
	void OnIAReload(const FInputActionValue& Value);

protected:

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	

	virtual void BeginPlay() override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	
	class APlayerController* PlayerController;

	// 플레이어가 마우스의 위치를 바라보게하는 함수
	void TurnPlayer();

	// 마우스의 위치
	FVector mousePos;

	// 마우스의 위치에 맞게 카메라를 이동시키는 함수
	void CameraMove();

	// 카메라 최초위치
	FVector cameraLoc;

	UPROPERTY()
	class AWeaponBase* Weapon;

	// AWeaponBase타입을 저장 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = TPS)
	TSubclassOf<AWeaponBase> WeaponClass;

	// 총 발사 시 실행되는 애니메이션 몽타주
	UPROPERTY(EditAnywhere, Category = TPS)
	class UAnimMontage* FireMontage;

	// 발사 모션 실행되는 기능
	void PlayFireMontage();

	// 재장전 애니메이션 몽타주
	UPROPERTY(EditAnywhere, Category = TPS)
	class UAnimMontage* ReloadMontage;

	// 재장전 모션 실행되는 기능
	void PlayReloadMontage();

	void OnMyReloadFinished();

	bool bReloading;

};

