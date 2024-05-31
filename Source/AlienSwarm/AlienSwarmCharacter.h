// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "HitInterface.h"
#include "AlienSwarmCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AAlienSwarmCharacter : public ACharacter,public IHitInterface
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

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IA_FirstWeapon;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IA_SecondWeapon;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IA_SubWeapon;

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

	// 1�� ����� �����ϴ� ���
	void OnIAFirstWeapon(const FInputActionValue& Value);

	// 2�� ����� �����ϴ� ���
	void OnIASecondWeapon(const FInputActionValue& Value);

	// ���� ����� �����ϴ� ���
	void OnIASubWeapon(const FInputActionValue& Value);

protected:

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	

	virtual void BeginPlay() override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	
	class ATestPlayerController* PlayerController;

	// �÷��̾ ���콺�� ��ġ�� �ٶ󺸰��ϴ� �Լ�
	void TurnPlayer();

	// ���콺�� ��ġ
	UPROPERTY(Replicated)
	FVector mousePos;

	// ���콺�� ��ġ�� �°� ī�޶� �̵���Ű�� �Լ�
	void CameraMove();

	// ī�޶� ������ġ
	FVector cameraLoc;

	UPROPERTY()
	class AWeaponBase* Weapon;

	// AWeaponBase Ÿ���� ���� / 1�� ����
 	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = TPS)
 	TSubclassOf<AWeaponBase> WeaponClass;

	UPROPERTY()
	class AWeaponBase* Weapon2;

	// AWeaponBase Ÿ���� ���� / 2�� ����
 	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = TPS)
 	TSubclassOf<AWeaponBase> WeaponClass2;

	UPROPERTY()
	class AWeaponBase* SubWeapon;

	// AWeaponBase3Ÿ���� ���� / ��������
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = TPS)
 	TSubclassOf<AWeaponBase> SubWeaponClass;

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

	// �������� ������ ��
	void OnMyReloadFinished();

	// ������ ������ �Ǻ��ϴ� ����
	bool bReloading;

	// ��� ������ �Ǻ��ϴ� ����
	bool bFireing;

	// ���⸦ �ٲٰ�ʹ�.
	void ChangeWeapon(AWeaponBase* ChangeWeapons);

	// ���� �����ϰ� �ִ� ���⸦ ������Ű�� �ʹ�.
	void DetachWeapon(AWeaponBase* Weapons);

	// ���⸦ �����Ѵ�.
	void SpawnWeapon();

	// ��� �������� �Ǻ���Ű�� ���� ������� ����
	int32 SelectedWeapon = 1;

	// �������� �޾��� �� ����Ǵ� ���
	virtual void TakeHit(int32 damage) override;
	
	class UMainWidget* mainWidget;

	class ATestPlayerController* testPC;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxHP = 1000;
	int32 HP = MaxHP;


	UFUNCTION(Server, Reliable)
	void ServerRPC_TakeDamage(int32 damage);

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_TakeDamage(int32 damage);

	
	// 1�� ���� ��ü �� �ٸ� Ŭ���忡�� ���� �� ����
	UFUNCTION(Server, Reliable)
	void ServerRPC_FirstWeapon();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_FirstWeapon();
	

	// 2�� ���� ��ü �� �ٸ� Ŭ���忡�� ���� �� ����
	UFUNCTION(Server, Reliable)
	void ServerRPC_SecondWeapon();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_SecondWeapon();

	// ���� ���� ��ü �� �ٸ� Ŭ���忡�� ���� �� ����
	UFUNCTION(Server, Reliable)
	void ServerRPC_SubWeapon();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_SubWeapon();
	

	// Ŭ���� ȸ��, ���콺 ��ġ�� ���� ���� �� ���� (�ӽ�)
 	UFUNCTION(Server, Reliable)
 	void ServerRPC_TurnPlayer(FVector _mousePos, FRotator _turn);

 	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_TurnPlayer(FVector _mousePos, FRotator _turn);


};

