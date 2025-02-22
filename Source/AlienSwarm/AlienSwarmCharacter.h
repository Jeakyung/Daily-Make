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

UENUM(BlueprintType)
enum class EWeaponType : uint8 {
	RIFLE,
	SHOTGUN,
	HEALGUN,
	GRANADE,
	ENGTOOL,
	BULLETBOX,
	HEALPACK,
};

USTRUCT(BlueprintType)
struct FWeaponInfo {
	GENERATED_BODY()

	EWeaponType weaponType;
	int32 damage;
	float shootingRange;
	float attackArea;
};

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AAlienSwarmCharacter : public ACharacter,public IHitInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
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

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IA_OpenGO;

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

	// GameOver UI 열기 버튼
	void OnIAOpenGO(const FInputActionValue& Value);

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
	FVector mousePos;

	// 마우스의 위치에 맞게 카메라를 이동시키는 함수
	void CameraMove();

	// 카메라 최초위치
	FVector cameraLoc;

	UPROPERTY(Replicated)
	class AWeaponBase* Weapon;

	// AWeaponBase 타입을 저장 / 1번 무기
 	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = TPS)
 	TSubclassOf<AWeaponBase> WeaponClass;

	UPROPERTY(Replicated)
	class AWeaponBase* Weapon2;

	// AWeaponBase 타입을 저장 / 2번 무기
 	UPROPERTY(Replicated,EditDefaultsOnly, BlueprintReadWrite, Category = TPS)
 	TSubclassOf<AWeaponBase> WeaponClass2;

	UPROPERTY(Replicated)
	class AWeaponBase* SubWeapon;

	// AWeaponBase3타입을 저장 / 보조무기
	UPROPERTY(Replicated,EditDefaultsOnly, BlueprintReadWrite, Category = TPS)
 	TSubclassOf<AWeaponBase> SubWeaponClass;

	struct FWeaponInfo weaponInfo1;
	struct FWeaponInfo weaponInfo2;
	struct FWeaponInfo subWeaponInfo;

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
	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnWeapon();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_SpawnWeapon();

	// 몇번 무기인지 판별시키기 위해 만들어진 변수
	UPROPERTY(Replicated)
	int32 SelectedWeapon = 1;

	// 데미지를 받았을 때 실행되는 기능
	virtual void TakeHit(int32 damage) override;
	
	class UMainWidget* mainWidget;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxHP = 1000;

	UPROPERTY(Replicated)
	int32 HP = MaxHP;


	// 데미지 받는 기능
	UFUNCTION(Server, Reliable)
	void ServerRPC_TakeDamage(int32 damage);

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_TakeDamage(float value);




	
	// 1번 무기 교체 시 다른 클라우드에도 변경 값 적용
	UFUNCTION(Server, Reliable)
	void ServerRPC_FirstWeapon(int _SelectedWeapon);

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_FirstWeapon(int _SelectedWeapon);
	//============================================



	// 2번 무기 교체 시 다른 클라우드에도 변경 값 적용
	UFUNCTION(Server, Reliable)
	void ServerRPC_SecondWeapon(int _SelectedWeapon);

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_SecondWeapon(int _SelectedWeapon);
	//============================================




	// 보조 무기 교체 시 다른 클라우드에도 변경 값 적용
	UFUNCTION(Server, Reliable)
	void ServerRPC_SubWeapon(int _SelectedWeapon);

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_SubWeapon(int _SelectedWeapon);
	//============================================





	// 클라우드 회전, 마우스 위치에 따른 조준 값 전달 
	UFUNCTION(Server, Reliable)
	void ServerRPC_TurnPlayer(FRotator newTurn);

	UPROPERTY(ReplicatedUsing=OnRep_TargetRotation)
	FRotator TargetRotation;

	UFUNCTION()
	void OnRep_TargetRotation();
	//============================================



	// 클라이언트 마우스에 따라 카메라 이동
	UFUNCTION(Server, Reliable)
	void ServerRPC_CameraMove(FVector _newPos);

	UPROPERTY(ReplicatedUsing = OnRep_CameraMove)
	FVector camMove;
	
	UFUNCTION()
	void OnRep_CameraMove();
	//============================================

	

	// 죽는 몽타주 생성
	UPROPERTY(EditAnywhere, Category = TPS)
	class UAnimMontage* DeathMontage;
	
	// HP가 0일때 죽는 기능
	void DiePlayer();

	// 죽었는지 판별하는 기능
	UPROPERTY(Replicated)
	bool bDie = false;
	//============================================


	class UGameOverWidget* GameOverWidget;

	UFUNCTION(Server, Reliable)
	void ServerRPC_FireRifle(FVector _start, FVector _end, int32 _damage);

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_FireRifle(FVector _start, FVector _end);

	UFUNCTION(Server, Reliable)
	void ServerRPC_FireShot(FVector _start, FVector _end, int32 _damage, float _attackArea);

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_FireShot(FVector _start, FVector _end, float _attackArea);

	UFUNCTION(Server, Reliable)
	void ServerRPC_FireHeal(FVector _start, FVector _end, int32 _damage);

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_FireHeal(FVector _start, FVector _end);

	UFUNCTION(Server, Reliable)
	void ServerRPC_FireGranade(FVector _mousePos);

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_FireGranade(FVector _mousePos);

	UFUNCTION(Server, Reliable)
	void ServerRPC_ToolEng(FVector _actorLoc);

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_ToolEng(FVector _actorLoc);

	UFUNCTION(Server, Reliable)
	void ServerRPC_ToolBullet(TSubclassOf<class AToolBulletBox> _BP_Bullet, FVector _setLoc);

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_ToolBullet(TSubclassOf<class AToolBulletBox> _BP_Bullet, FVector _setLoc);

	UFUNCTION(Server, Reliable)
	void ServerRPC_ToolHeal(TSubclassOf<class AToolHealPack> _BP_Heal, FVector _setLoc);

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_ToolHeal(TSubclassOf<class AToolHealPack> _BP_Heal, FVector _setLoc);

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* rifleSound;

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* shotSound;

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* healSound;
};

