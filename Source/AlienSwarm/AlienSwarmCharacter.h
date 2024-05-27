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

	// �߻��ϴ� ���
	void OnIAFire(const FInputActionValue& Value);
	
	// �������ϴ� ���
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

	// �÷��̾ ���콺�� ��ġ�� �ٶ󺸰��ϴ� �Լ�
	void TurnPlayer();

	// ���콺�� ��ġ
	FVector mousePos;

	// ���콺�� ��ġ�� �°� ī�޶� �̵���Ű�� �Լ�
	void CameraMove();

	// ī�޶� ������ġ
	FVector cameraLoc;

	UPROPERTY()
	class AWeaponBase* Weapon;

	// AWeaponBaseŸ���� ���� 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = TPS)
	TSubclassOf<AWeaponBase> WeaponClass;

	// �� �߻� �� ����Ǵ� �ִϸ��̼� ��Ÿ��
	UPROPERTY(EditAnywhere, Category = TPS)
	class UAnimMontage* FireMontage;

	// �߻� ��� ����Ǵ� ���
	void PlayFireMontage();

	// ������ �ִϸ��̼� ��Ÿ��
	UPROPERTY(EditAnywhere, Category = TPS)
	class UAnimMontage* ReloadMontage;

	// ������ ��� ����Ǵ� ���
	void PlayReloadMontage();

	void OnMyReloadFinished();

	bool bReloading;

};

