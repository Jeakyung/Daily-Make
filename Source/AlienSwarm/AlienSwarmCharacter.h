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

	// 발사하는 기능
	void OnIAFire(const FInputActionValue& Value);
	
	// 재장전하는 기능
	void OnIAReload(const FInputActionValue& Value);

	// 1번 무기로 변경하는 기능
	void OnIAFirstWeapon(const FInputActionValue& Value);

	// 2번 무기로 변경하는 기능
	void OnIASecondWeapon(const FInputActionValue& Value);

	// 보조 무기로 변경하는 기능
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

	// 플레이어가 마우스의 위치를 바라보게하는 함수
	void TurnPlayer();

	// 마우스의 위치
	UPROPERTY(Replicated)
	FVector mousePos;

	// 마우스의 위치에 맞게 카메라를 이동시키는 함수
	void CameraMove();

	// 카메라 최초위치
	FVector cameraLoc;

	UPROPERTY()
	class AWeaponBase* Weapon;

	// AWeaponBase 타입을 저장 / 1번 무기
 	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = TPS)
 	TSubclassOf<AWeaponBase> WeaponClass;

	UPROPERTY()
	class AWeaponBase* Weapon2;

	// AWeaponBase 타입을 저장 / 2번 무기
 	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = TPS)
 	TSubclassOf<AWeaponBase> WeaponClass2;

	UPROPERTY()
	class AWeaponBase* SubWeapon;

	// AWeaponBase3타입을 저장 / 보조무기
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = TPS)
 	TSubclassOf<AWeaponBase> SubWeaponClass;

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

	// 재장전이 끝났을 때
	void OnMyReloadFinished();

	// 재장전 중인지 판별하는 변수
	bool bReloading;

	// 사격 중인지 판별하는 변수
	bool bFireing;

	// 무기를 바꾸고싶다.
	void ChangeWeapon(AWeaponBase* ChangeWeapons);

	// 현재 장착하고 있는 무기를 해제시키고 싶다.
	void DetachWeapon(AWeaponBase* Weapons);

	// 무기를 스폰한다.
	void SpawnWeapon();

	// 몇번 무기인지 판별시키기 위해 만들어진 변수
	int32 SelectedWeapon = 1;

	// 데미지를 받았을 때 실행되는 기능
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

	
	// 1번 무기 교체 시 다른 클라우드에도 변경 값 적용
	UFUNCTION(Server, Reliable)
	void ServerRPC_FirstWeapon();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_FirstWeapon();
	

	// 2번 무기 교체 시 다른 클라우드에도 변경 값 적용
	UFUNCTION(Server, Reliable)
	void ServerRPC_SecondWeapon();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_SecondWeapon();

	// 보조 무기 교체 시 다른 클라우드에도 변경 값 적용
	UFUNCTION(Server, Reliable)
	void ServerRPC_SubWeapon();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_SubWeapon();
	

	// 클라우드 회전, 마우스 위치에 따른 조준 값 전달 (임시)
 	UFUNCTION(Server, Reliable)
 	void ServerRPC_TurnPlayer(FVector _mousePos, FRotator _turn);

 	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_TurnPlayer(FVector _mousePos, FRotator _turn);


};

