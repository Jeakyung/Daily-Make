// Copyright Epic Games, Inc. All Rights Reserved.

#include "AlienSwarmCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include <../../../../../../../Source/Runtime/Engine/Public/CollisionQueryParams.h>
#include "WeaponBase.h"
#include "PlayerAnimInstance.h"
#include "HitInterface.h"
#include "TestPlayerController.h"
#include "MainWidget.h"
#include "Net/UnrealNetwork.h"
#include "NiagaraComponent.h"
#include "MainGameModeBase.h"
#include "GameOverWidget.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PlayerController.h>
#include "GameOverWidget.h"
#include "DoorActor.h"
#include "ToolBulletBox.h"
#include "ToolHealPack.h"
#include "Kismet/GamePlayStatics.h"



DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AAlienSwarmCharacter

AAlienSwarmCharacter::AAlienSwarmCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them

	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller



	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	FollowCamera->SetRelativeLocation(FVector(-42,0,890));
	FollowCamera->SetRelativeRotation(FRotator(-60,0,0));


	bReplicates = true;
	SetReplicateMovement(true);

}

void AAlienSwarmCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	auto* pc = Cast<APlayerController>(Controller);
	if (pc)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	PlayerController = Cast<ATestPlayerController>(Controller);
	// 카메라 최초 위치
	cameraLoc = FollowCamera->GetRelativeLocation();
	
	//현재 열려있는 맵의 이름이 TitleLevel이면(실제 리턴되는 값은 UEDPIE_0_TitleLevel임)
	if (IsLocallyControlled() && PlayerController) {
		if (GetWorld()->GetMapName().Contains(TEXT("TitleLevel"))) {
			PlayerController->MakeTitleWidget();
		}
		else if (GetWorld()->GetMapName().Contains(TEXT("TestLevel"))) {
			PlayerController->MakeShopWidget();
		}
		else {
			PlayerController->MakeMainWidget();
		}
	}



	

	//PlayerController->SetShowMouseCursor(true);
}


void AAlienSwarmCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	TurnPlayer();
	CameraMove();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AAlienSwarmCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAlienSwarmCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAlienSwarmCharacter::Look);
		
		// Fire
		EnhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Triggered, this, &AAlienSwarmCharacter::OnIAFire);

		// Reload
		EnhancedInputComponent->BindAction(IA_Reload, ETriggerEvent::Started, this, &AAlienSwarmCharacter::OnIAReload);

		// MainWeapon 1
		EnhancedInputComponent->BindAction(IA_FirstWeapon, ETriggerEvent::Started, this, &AAlienSwarmCharacter::OnIAFirstWeapon);

		// MainWeapon 2
		EnhancedInputComponent->BindAction(IA_SecondWeapon, ETriggerEvent::Started, this, &AAlienSwarmCharacter::OnIASecondWeapon);

		// SubWeapon
		EnhancedInputComponent->BindAction(IA_SubWeapon, ETriggerEvent::Started, this, &AAlienSwarmCharacter::OnIASubWeapon);
		
		// Open GameOver UI
		EnhancedInputComponent->BindAction(IA_OpenGO, ETriggerEvent::Started, this, &AAlienSwarmCharacter::OnIAOpenGO);

	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AAlienSwarmCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AAlienSwarmCharacter::Look(const FInputActionValue& Value)
{
	

}

void AAlienSwarmCharacter::OnIAFire(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Shooting!!"));
	if (bReloading == false)
	{
		switch (SelectedWeapon)
		{
		case 1:
			if (nullptr != Weapon) 
			{
				FVector start = Weapon->firePoint->GetComponentLocation();
				FVector end = start + Weapon->firePoint->GetForwardVector() * weaponInfo1.shootingRange;
				end.Z = start.Z - 150.0f;
				switch (weaponInfo1.weaponType)
				{
				case EWeaponType::RIFLE :
					if (Weapon->OnFire(mousePos)) {
						ServerRPC_FireRifle(start, end, weaponInfo1.damage);
					}
					break;
				case EWeaponType::SHOTGUN :
					if (Weapon->OnFire(mousePos)) {
						ServerRPC_FireShot(start, end, weaponInfo1.damage, weaponInfo1.attackArea);
					}
					break;
				case EWeaponType::HEALGUN :
					if (Weapon->OnFire(mousePos)) {
						ServerRPC_FireHeal(start, end, weaponInfo1.damage);
					}
					break;
				default:
					break;
				}
				PlayFireMontage();
			}
			break;
		case 2:
			if (nullptr != Weapon2)
			{
				FVector start = Weapon2->firePoint->GetComponentLocation();
				FVector end = start + Weapon2->firePoint->GetForwardVector() * weaponInfo2.shootingRange;
				end.Z = start.Z - 150.0f;
				switch (weaponInfo2.weaponType)
				{
				case EWeaponType::RIFLE:
					if (Weapon->OnFire(mousePos)) {
						ServerRPC_FireRifle(start, end, weaponInfo2.damage);
					}
					break;
				case EWeaponType::SHOTGUN:
					if (Weapon->OnFire(mousePos)) {
						ServerRPC_FireShot(start, end, weaponInfo2.damage, weaponInfo2.attackArea);
					}
					break;
				case EWeaponType::HEALGUN:
					if (Weapon->OnFire(mousePos)) {
						ServerRPC_FireHeal(start, end, weaponInfo2.damage);
					}
					break;
				default:
					break;
				}
				PlayFireMontage();
			}
			break;
		case 3:
			if (nullptr != SubWeapon)
			{
				switch (subWeaponInfo.weaponType)
				{
				case EWeaponType::GRANADE:
					if (SubWeapon->OnFire(mousePos)) {
						ServerRPC_FireGranade(mousePos);
					}
					break;
				case EWeaponType::ENGTOOL:
					if (SubWeapon->OnFire(mousePos)) {
						ServerRPC_ToolEng(GetActorLocation());
					}
					break;
				case EWeaponType::BULLETBOX:
					if (SubWeapon->OnFire(mousePos)) {
						ServerRPC_ToolBullet(Cast<AToolBulletBox>(SubWeapon)->bulletBox_BP, GetActorLocation());
					}
					break;
				case EWeaponType::HEALPACK:
					if (SubWeapon->OnFire(mousePos)) {
						ServerRPC_ToolHeal(Cast<AToolHealPack>(SubWeapon)->healPack_BP, GetActorLocation());
					}
					break;
				default:
					break;
				}
				PlayFireMontage();
			}
			break;
		
		}
		
	}
}

void AAlienSwarmCharacter::OnIAReload(const FInputActionValue& Value)
{
	PlayReloadMontage();
	
}


void AAlienSwarmCharacter::OnIAOpenGO(const FInputActionValue& Value)
{
	PlayerController->MakeGameOverWidget();
}

void AAlienSwarmCharacter::TurnPlayer()
{
	if (IsLocallyControlled())
	{
		auto* pc = Cast<ATestPlayerController>(Controller);
		if (nullptr == pc)
		{
			return;
		}

		FVector mouseLocation, mouseDirection;
		mouseDirection.Normalize();
		// 플레이어의 마우스 위치와 방향 값을 각각 mouseLocation과 mouseDirection에 넣어준다.
		pc->DeprojectMousePositionToWorld(mouseLocation, mouseDirection);
		
		// 라인 그리기
		// start : 카메라 위치
		FVector start = FollowCamera->GetComponentLocation();
		// end = 화면상에 마우스가 가르키는 위치 
		FVector end = start + mouseDirection * 100000;
		// 플레이어와의 충돌 무시
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);

		FHitResult hitResult;
		// 부딪힌 것이 있는지 판별하는 변수 
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility, params);

		mousePos = hitResult.ImpactPoint;
		


		// 방향 = 마우스 위치에 쏘여진 라인트레이스와 충돌한 위치 - 플레이어 위치
		FVector direction = mousePos - GetActorLocation();
		FRotator turnDir = direction.Rotation();
		FRotator turn = FRotator(0, turnDir.Yaw + 10, 0);
		
		DrawDebugSphere(GetWorld(), mousePos, 50.0f, 3, FColor::Red, false, 0, 0, 1);

		//this->SetActorRotation(turn);

		ServerRPC_TurnPlayer(turn);
		
		//MultiRPC_TurnPlayer(mousePos, turn);
		


	}
}

void AAlienSwarmCharacter::CameraMove()
{
	if (IsLocallyControlled())
	{

		auto* pc = Cast<ATestPlayerController>(Controller);
		if (nullptr == pc)
		{
			return;
		}
		double distance = FVector::Distance(GetActorLocation(), mousePos);



		FVector mouseLoc = mousePos - GetActorLocation() + cameraLoc;
		mouseLoc.Z = FollowCamera->GetRelativeLocation().Z;
		//mouseLoc.X -= 300.0f;

		FVector newPos = FMath::Lerp(cameraLoc, mouseLoc, 0.2);
		newPos.X -= 300;

		
		ServerRPC_CameraMove(newPos);


	}
}

void AAlienSwarmCharacter::PlayFireMontage()
{

	auto* anim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (anim)
	{
			anim->Montage_Play(FireMontage);
	}
}

void AAlienSwarmCharacter::PlayReloadMontage()
{
	auto* anim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (anim)
	{
		if (bReloading == false)
		{ 
			anim->Montage_Play(ReloadMontage);
			bReloading = true;
			
		}
	}
	
}

// 재장전이 끝날떄
void AAlienSwarmCharacter::OnMyReloadFinished()
{
	
	
	switch (SelectedWeapon)
	{
	case 1:
		if (Weapon)
		{
			Weapon->OnReload();
		}
		break;
	case 2:
		if (Weapon2)
		{
			Weapon2->OnReload();
		}
		break;
	case 3:
		if (SubWeapon)
		{
			SubWeapon->OnReload();
		}
		break;

	}
	bReloading = false;
	UE_LOG(LogTemp, Warning, TEXT("Reload"));
}


// 무기 스폰하기
void AAlienSwarmCharacter::ServerRPC_SpawnWeapon_Implementation()
{
	MultiRPC_SpawnWeapon();
}

void AAlienSwarmCharacter::MultiRPC_SpawnWeapon_Implementation()
{
	if (WeaponClass) {
		// 1번 무기 생성
		Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass);
		Weapon->SetActorLocation(FVector(0, 0, -30000));
		Weapon->SetOwner(this);
		if (Weapon->ActorHasTag(TEXT("Rifle"))) {
			weaponInfo1.weaponType = EWeaponType::RIFLE;
		}
		else if (Weapon->ActorHasTag(TEXT("Shotgun"))) {
			weaponInfo1.weaponType = EWeaponType::SHOTGUN;
		}
		else if (Weapon->ActorHasTag(TEXT("HealGun"))) {
			weaponInfo1.weaponType = EWeaponType::HEALGUN;
		}

		weaponInfo1.damage = Weapon->GetDamage();
		weaponInfo1.shootingRange = Weapon->GetShootingRange();
		weaponInfo1.attackArea = Weapon->GetAttackArea();
	}

	if (WeaponClass2) {
		// 2번 무기 생성
		Weapon2 = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass2);
		Weapon2->SetActorLocation(FVector(0, 0, -30000));
		Weapon2->SetOwner(this);
		if (Weapon2->ActorHasTag(TEXT("Rifle"))) {
			weaponInfo2.weaponType = EWeaponType::RIFLE;
		}
		else if (Weapon2->ActorHasTag(TEXT("Shotgun"))) {
			weaponInfo2.weaponType = EWeaponType::SHOTGUN;
		}
		else if (Weapon2->ActorHasTag(TEXT("HealGun"))) {
			weaponInfo2.weaponType = EWeaponType::HEALGUN;
		}

		weaponInfo2.damage = Weapon2->GetDamage();
		weaponInfo2.shootingRange = Weapon2->GetShootingRange();
		weaponInfo2.attackArea = Weapon2->GetAttackArea();
	}

	if (SubWeaponClass) {
		// 보조 무기 생성
		SubWeapon = GetWorld()->SpawnActor<AWeaponBase>(SubWeaponClass);
		SubWeapon->SetActorLocation(FVector(0, 0, -30000));
		SubWeapon->SetOwner(this);
		if (SubWeapon->ActorHasTag(TEXT("Granade"))) {
			subWeaponInfo.weaponType = EWeaponType::GRANADE;
		}
		else if (SubWeapon->ActorHasTag(TEXT("ENGTool"))) {
			subWeaponInfo.weaponType = EWeaponType::ENGTOOL;
		}
		else if (SubWeapon->ActorHasTag(TEXT("BulletBox"))) {
			subWeaponInfo.weaponType = EWeaponType::BULLETBOX;
		}
		else if (SubWeapon->ActorHasTag(TEXT("HealPack"))) {
			subWeaponInfo.weaponType = EWeaponType::HEALPACK;
		}
	}
}

// 데미지를 받았을 때 실행되는 기능
void AAlienSwarmCharacter::TakeHit(int32 damage)
{
	ServerRPC_TakeDamage(damage);

}


// damage
void AAlienSwarmCharacter::ServerRPC_TakeDamage_Implementation(int32 damage)
{
	

		HP = FMath::Clamp(HP, 0, MaxHP);


		int32 truedamage = damage;

		HP -= truedamage;


		float hpratio = (float)HP / (float)MaxHP;

		

		float Test =hpratio;

		MultiRPC_TakeDamage(Test);


		UE_LOG(LogTemp, Warning, TEXT("%d"), HP);

}


void AAlienSwarmCharacter::MultiRPC_TakeDamage_Implementation(float value)
{
	if (IsLocallyControlled())
	{

		auto* pc = Cast<ATestPlayerController>(Controller);
		if (nullptr == pc)
		{
			return;
		}

	
		PlayerController->SetHP(value);

		if (HP <= 1)
		{
			DiePlayer();
		}
	}
}



// 1번 무기로 변경하는 기능
void AAlienSwarmCharacter::OnIAFirstWeapon(const FInputActionValue& Value)
{
	SelectedWeapon = 1;
	ServerRPC_FirstWeapon(SelectedWeapon);

	UE_LOG(LogTemp, Warning, TEXT("FirstWeapon"));
}

// 2번 무기로 변경하는 기능
void AAlienSwarmCharacter::OnIASecondWeapon(const FInputActionValue& Value)
{
	SelectedWeapon = 2;
	ServerRPC_SecondWeapon(SelectedWeapon);
	UE_LOG(LogTemp, Warning, TEXT("SecoandWeapon"));
}

// 보조 무기로 변경하는 기능
void AAlienSwarmCharacter::OnIASubWeapon(const FInputActionValue& Value)
{
	SelectedWeapon = 3;
	ServerRPC_SubWeapon(SelectedWeapon);

	UE_LOG(LogTemp, Warning, TEXT("SubWeapon"));
}



// 무기 장착 시키기 (무기 교체 시 사용됨)
void AAlienSwarmCharacter::ChangeWeapon(AWeaponBase* ChangeWeapons)
{

	// 만약 생성이 유효하다면
	if (nullptr != ChangeWeapons)
	{
		// 총을 플레이어의 손에 부착
		ChangeWeapons->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("RightHandSocket"));
		ChangeWeapons->SetActorRelativeLocation(FVector(4.7f, -11, 1.8f));
		ChangeWeapons->SetActorRelativeRotation(FRotator(-11, 81, -81));
		ChangeWeapons->SetActorRelativeScale3D(FVector(1.25f));
		ChangeWeapons->Equip(this);
		ChangeWeapons->aimmingLaser->Activate(true);
		UE_LOG(LogTemp, Warning, TEXT("ChangeWeapon"));
		ChangeWeapons->SetActorHiddenInGame(false);
	}

}

// 무기 장착 해제시키기
void AAlienSwarmCharacter::DetachWeapon(AWeaponBase* Weapons)
{
	if (Weapons)
	{
		Weapons->SetActorHiddenInGame(true);
	}
}

// 1번 무기 교체 시 다른 클라우드에도 변경 값 적용
void AAlienSwarmCharacter::ServerRPC_FirstWeapon_Implementation(int _SelectedWeapon)
{
	SelectedWeapon = _SelectedWeapon;
	MultiRPC_FirstWeapon(_SelectedWeapon);

}
void AAlienSwarmCharacter::MultiRPC_FirstWeapon_Implementation(int _SelectedWeapon)
{
	SelectedWeapon = _SelectedWeapon;
	DetachWeapon(SubWeapon);
	DetachWeapon(Weapon2);


	ChangeWeapon(Weapon);
}
//============================================

// 2번 무기 교체 시 다른 클라우드에도 변경 값 적용
void AAlienSwarmCharacter::ServerRPC_SecondWeapon_Implementation(int _SelectedWeapon)
{
	SelectedWeapon = _SelectedWeapon;
	MultiRPC_SecondWeapon(_SelectedWeapon);

}
void AAlienSwarmCharacter::MultiRPC_SecondWeapon_Implementation(int _SelectedWeapon)
{
	SelectedWeapon = _SelectedWeapon;
	DetachWeapon(SubWeapon);
	DetachWeapon(Weapon);


	ChangeWeapon(Weapon2);
}
//============================================



// 보조 무기 교체 시 다른 클라우드에도 변경 값 적용
void AAlienSwarmCharacter::ServerRPC_SubWeapon_Implementation(int _SelectedWeapon)
{
	SelectedWeapon = _SelectedWeapon;
	MultiRPC_SubWeapon(_SelectedWeapon);
}

void AAlienSwarmCharacter::MultiRPC_SubWeapon_Implementation(int _SelectedWeapon)
{
	SelectedWeapon = _SelectedWeapon;
	DetachWeapon(Weapon2);
	DetachWeapon(Weapon);


	ChangeWeapon(SubWeapon);
}
//============================================


// 클라우드 회전, 마우스 위치에 따른 조준 값 전달 
void AAlienSwarmCharacter::ServerRPC_TurnPlayer_Implementation(FRotator newTurn)
{
	TargetRotation = newTurn;
	OnRep_TargetRotation();
}

void AAlienSwarmCharacter::OnRep_TargetRotation()
{
	SetActorRotation(TargetRotation);
}
//============================================


// 클라이언트 마우스에 따라 카메라 이동
void AAlienSwarmCharacter::ServerRPC_CameraMove_Implementation(FVector _newPos)
{
	camMove = _newPos;
	OnRep_CameraMove();
}

void AAlienSwarmCharacter::OnRep_CameraMove()
{
	FollowCamera->SetRelativeLocation(camMove);
}
//============================================


// 피 0되면 죽는 기능
void AAlienSwarmCharacter::DiePlayer()
{
	if (IsLocallyControlled())
	{
		if (HP <= 0)
		{
			// 피 0이 되면 bDie를 true로 만들고 애니메이션 실행
			bDie = true;

			// 움직임 멈춤
			GetCharacterMovement()->DisableMovement();

			// 관전 모드로 되기 전에 무기 없애기
			DetachWeapon(Weapon);
			DetachWeapon(Weapon2);
			DetachWeapon(SubWeapon);

			PlayerController->MakeGameOverWidget();

			// 관전 모드 실행
			PlayerController->ServerRPC_ChangeSpectator();
		}

	}
}

void AAlienSwarmCharacter::ServerRPC_FireRifle_Implementation(FVector _start, FVector _end, int32 _damage)
{
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.AddIgnoredActor(GetOwner());
	FHitResult hitInfo;
	//Block되는 채널을 찾음
	bool bResult = GetWorld()->LineTraceSingleByChannel(hitInfo, _start, _end, ECC_GameTraceChannel1, params);
	if (bResult) {
		IHitInterface* target = Cast<IHitInterface>(hitInfo.GetActor());
		if (target) {
			target->TakeHit(_damage);
		}
	}
	MultiRPC_FireRifle(_start, _end);
}

void AAlienSwarmCharacter::MultiRPC_FireRifle_Implementation(FVector _start, FVector _end)
{
	DrawDebugLine(GetWorld(), _start, _end, FColor::Red, false, 5.0f);
	UGameplayStatics::PlaySound2D(GetWorld(), rifleSound);
}

void AAlienSwarmCharacter::ServerRPC_FireShot_Implementation(FVector _start, FVector _end, int32 _damage, float _attackArea)
{
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.AddIgnoredActor(GetOwner());
	TArray<FHitResult> hitInfos;
	//Ignore되는 채널을 찾음
	bool bResult = GetWorld()->SweepMultiByChannel(hitInfos, _start, _end, FQuat::Identity, ECC_GameTraceChannel2, FCollisionShape::MakeSphere(_attackArea), params);
	if (bResult) {
		for (FHitResult& hit : hitInfos) {
			IHitInterface* target = Cast<IHitInterface>(hit.GetActor());
			if (target) {
				target->TakeHit(_damage);
			}
		}
	}
	MultiRPC_FireShot(_start, _end, _attackArea);
}

void AAlienSwarmCharacter::MultiRPC_FireShot_Implementation(FVector _start, FVector _end, float _attackArea)
{
	DrawDebugCylinder(GetWorld(), _start, _end, _attackArea, 32, FColor::Red, false, 5.0f);
	UGameplayStatics::PlaySound2D(GetWorld(), shotSound);
}

void AAlienSwarmCharacter::ServerRPC_FireHeal_Implementation(FVector _start, FVector _end, int32 _damage)
{
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.AddIgnoredActor(GetOwner());
	FHitResult hitInfo;
	//Block되는 채널을 찾음
	bool bResult = GetWorld()->LineTraceSingleByChannel(hitInfo, _start, _end, ECC_GameTraceChannel1, params);
	if (bResult) {
		IHitInterface* target = Cast<IHitInterface>(hitInfo.GetActor());
		if (target) {
			target->TakeHit(-_damage);
		}
	}
	MultiRPC_FireRifle(_start, _end);
}

void AAlienSwarmCharacter::MultiRPC_FireHeal_Implementation(FVector _start, FVector _end)
{
	DrawDebugLine(GetWorld(), _start, _end, FColor::Green, false, 5.0f);
	UGameplayStatics::PlaySound2D(GetWorld(), healSound);
}

void AAlienSwarmCharacter::ServerRPC_FireGranade_Implementation(FVector _mousePos)
{
	TArray<FOverlapResult> hitsInfos;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	bool bResult = GetWorld()->OverlapMultiByChannel(hitsInfos, _mousePos, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(250.0f), params);
	if (bResult) {
		for (FOverlapResult& hit : hitsInfos) {
			IHitInterface* target = Cast<IHitInterface>(hit.GetActor());
			if (target) {
				target->TakeHit(50);
			}
		}
	}
	MultiRPC_FireGranade(_mousePos);
}

void AAlienSwarmCharacter::MultiRPC_FireGranade_Implementation(FVector _mousePos)
{
	DrawDebugSphere(GetWorld(), _mousePos, 250.0f, 32, FColor::Red, false, 3.0f, 0, 2.0f);
}

void AAlienSwarmCharacter::ServerRPC_ToolEng_Implementation(FVector _actorLoc)
{
	TArray<FOverlapResult> hitsInfos;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	bool bResult = GetWorld()->OverlapMultiByChannel(hitsInfos, _actorLoc, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(150.0f), params);

	if (bResult) {
		for (FOverlapResult& hit : hitsInfos) {
			ADoorActor* target = Cast<ADoorActor>(hit.GetActor());
			if (target) {
				UE_LOG(LogTemp, Warning, TEXT("EngTool : Find Door"));
				if (!target->bIsLocked) {
					target->DoorLock();
					if (target->bIsOpened) {
						target->ServerRPC_DoorClose();
					}
				}
				else {
					target->DoorUnLock();
					if (!target->bIsOpened) {
						target->ServerRPC_DoorOpen();
					}
				}
				break;
			}
		}
	}
}

void AAlienSwarmCharacter::MultiRPC_ToolEng_Implementation(FVector _actorLoc)
{
	DrawDebugSphere(GetWorld(), _actorLoc, 150.0f, 32, FColor::Blue, false, 3.0f, 0, 2.0f);
}

void AAlienSwarmCharacter::ServerRPC_ToolBullet_Implementation(TSubclassOf<AToolBulletBox> _BP_Bullet, FVector _setLoc)
{
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AToolBulletBox* setBox = GetWorld()->SpawnActor<AToolBulletBox>(_BP_Bullet, _setLoc, FRotator::ZeroRotator, params);
	setBox->bSet = true;

	//MultiRPC_ToolBullet(_BP_Bullet, _setLoc);
}

void AAlienSwarmCharacter::MultiRPC_ToolBullet_Implementation(TSubclassOf<AToolBulletBox> _BP_Bullet, FVector _setLoc)
{
	/*FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AToolBulletBox* setBox = GetWorld()->SpawnActor<AToolBulletBox>(_BP_Bullet, _setLoc, FRotator::ZeroRotator, params);
	setBox->bSet = true;*/
}

void AAlienSwarmCharacter::ServerRPC_ToolHeal_Implementation(TSubclassOf<AToolHealPack> _BP_Heal, FVector _setLoc)
{
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AToolHealPack* setBox = GetWorld()->SpawnActor<AToolHealPack>(_BP_Heal, _setLoc, FRotator::ZeroRotator, params);
	setBox->bSet = true;

	//MultiRPC_ToolHeal(_BP_Heal, _setLoc);
}

void AAlienSwarmCharacter::MultiRPC_ToolHeal_Implementation(TSubclassOf<AToolHealPack> _BP_Heal, FVector _setLoc)
{
	/*FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AToolHealPack* setBox = GetWorld()->SpawnActor<AToolHealPack>(_BP_Heal, _setLoc, FRotator::ZeroRotator, params);
	setBox->bSet = true;*/
}

void AAlienSwarmCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// 플레이어 회전 동기화
	DOREPLIFETIME(AAlienSwarmCharacter, TargetRotation);
	// 마우스 이동에 따른 카메라 이동 동기화
	DOREPLIFETIME(AAlienSwarmCharacter, camMove);
	// 죽음처리 동기화
	DOREPLIFETIME(AAlienSwarmCharacter, bDie);
	// HP 동기화
	DOREPLIFETIME(AAlienSwarmCharacter, HP);

// 	DOREPLIFETIME(AAlienSwarmCharacter, Weapon);
// 	DOREPLIFETIME(AAlienSwarmCharacter, Weapon2);
// 	DOREPLIFETIME(AAlienSwarmCharacter, SubWeapon);
	DOREPLIFETIME(AAlienSwarmCharacter, SelectedWeapon);

}