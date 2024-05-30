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


}

void AAlienSwarmCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	// ī�޶� ���� ��ġ
	cameraLoc = FollowCamera->GetRelativeLocation();
	

	SpawnWeapon();


	ChangeWeapon(Weapon);

	//PlayerController->SetShowMouseCursor(true);
}


void AAlienSwarmCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	TurnPlayer();
	CameraMove();

	if (nullptr != Weapon)
	{
		Weapon->CalculateEndPoint(mousePos);
	}
	if (nullptr != Weapon2)
	{
		Weapon2->CalculateEndPoint(mousePos);
	}
	if (nullptr != SubWeapon)
	{
		SubWeapon->CalculateEndPoint(mousePos);
	}
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
				Weapon->OnFire(mousePos);
				PlayFireMontage();
			}
			break;
		case 2:
			if (nullptr != Weapon2)
			{
				Weapon2->OnFire(mousePos);
				PlayFireMontage();
			}
			break;
		case 3:
			if (nullptr != SubWeapon)
			{
				SubWeapon->OnFire(mousePos);
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

// 1�� ����� �����ϴ� ���
void AAlienSwarmCharacter::OnIAFirstWeapon(const FInputActionValue& Value)
{
	SelectedWeapon = 1;
	ServerRPC_FirstWeapon();

	UE_LOG(LogTemp, Warning, TEXT("FirstWeapon"));
}

// 2�� ����� �����ϴ� ���
void AAlienSwarmCharacter::OnIASecondWeapon(const FInputActionValue& Value)
{
	SelectedWeapon = 2;
	ServerRPC_SecondWeapon();
	UE_LOG(LogTemp, Warning, TEXT("SecoandWeapon"));
}

// ���� ����� �����ϴ� ���
void AAlienSwarmCharacter::OnIASubWeapon(const FInputActionValue& Value)
{
	SelectedWeapon = 3;
	ServerRPC_SubWeapon();
	
	UE_LOG(LogTemp, Warning, TEXT("SubWeapon"));
}

void AAlienSwarmCharacter::TurnPlayer()
{
	if (nullptr != Controller)
	{
		
		FVector mouseLocation, mouseDirection;
		mouseDirection.Normalize();
		PlayerController = Cast<APlayerController>(GetOwner());
		// �÷��̾��� ���콺 ��ġ�� ���� ���� ���� mouseLocation�� mouseDirection�� �־��ش�.
		PlayerController->DeprojectMousePositionToWorld(mouseLocation, mouseDirection);
		
		// ���� �׸���
		// start : ī�޶� ��ġ
		FVector start = FollowCamera->GetComponentLocation();
		// end = ȭ��� ���콺�� ����Ű�� ��ġ 
		FVector end = start + mouseDirection * 100000;
		// �÷��̾���� �浹 ����
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);
		UE_LOG(LogTemp, Warning, TEXT("%f,%f,%f"),mouseLocation.X, mouseLocation.Y, mouseLocation.Z);
		FHitResult hitResult;
		// �ε��� ���� �ִ��� �Ǻ��ϴ� ���� 
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility, params);

		mousePos = hitResult.ImpactPoint;
		


		// ���� = ���콺 ��ġ�� ��� ����Ʈ���̽��� �浹�� ��ġ - �÷��̾� ��ġ
		FVector direction = mousePos - GetActorLocation();
		FRotator turnDir = direction.Rotation();
		FRotator turn = FRotator(0, turnDir.Yaw + 10, 0);
		
		

		ServerRPC_TurnPlayer(mousePos, turn);


	}
}

void AAlienSwarmCharacter::CameraMove()
{
	double distance = FVector::Distance(GetActorLocation(),mousePos); 

	

	FVector mouseLoc = mousePos - GetActorLocation()+cameraLoc;
	mouseLoc.Z = FollowCamera->GetRelativeLocation().Z;
	//mouseLoc.X -= 300.0f;

	FVector newPos = FMath::Lerp(cameraLoc, mouseLoc, 0.2);
	newPos.X -= 300; 

	
	FollowCamera->SetRelativeLocation(newPos);




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

// �������� ������
void AAlienSwarmCharacter::OnMyReloadFinished()
{
	
	
	switch (SelectedWeapon)
	{
	case 1:
		Weapon->OnReload();
		break;
	case 2:
		Weapon2->OnReload();
		break;
	case 3:
		SubWeapon->OnReload();

		break;

	}
	bReloading = false;
	UE_LOG(LogTemp, Warning, TEXT("Reload"));
}

// ���� ���� ��Ű�� (���� ��ü �� ����)
void AAlienSwarmCharacter::ChangeWeapon(AWeaponBase* ChangeWeapons)
{
	
		// ���� ������ ��ȿ�ϴٸ�
	if (nullptr != ChangeWeapons)
	{
			// ���� �÷��̾��� �տ� ����
		ChangeWeapons->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("RightHandSocket"));
		ChangeWeapons->SetActorRelativeLocation(FVector(4.7f, -11, 1.8f));
		ChangeWeapons->SetActorRelativeRotation(FRotator(-11, 81, -81));
		ChangeWeapons->SetActorRelativeScale3D(FVector(1.25f));
		ChangeWeapons->Equip(this);
		UE_LOG(LogTemp, Warning, TEXT("spawnWeapon1"));
			
	}
	
}

// ���� ���� ������Ű��
void AAlienSwarmCharacter::DetachWeapon(AWeaponBase* Weapons)
{
	if (Weapons)
	{
		Weapons->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Weapons->SetActorLocation(FVector(0,0,-30000));
	}
}

// ���� �����ϱ�
void AAlienSwarmCharacter::SpawnWeapon()
{
	if (WeaponClass) {
		// 1�� ���� ����
		Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass);
		Weapon->SetActorLocation(FVector(0, 0, -30000));
	}

	if (WeaponClass2) {
		// 2�� ���� ����
		Weapon2 = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass2);
		Weapon2->SetActorLocation(FVector(0, 0, -30000));
	}

	if (SubWeaponClass) {
		// ���� ���� ����
		SubWeapon = GetWorld()->SpawnActor<AWeaponBase>(SubWeaponClass);
		SubWeapon->SetActorLocation(FVector(0, 0, -30000));
	}
}

// �������� �޾��� �� ����Ǵ� ���
void AAlienSwarmCharacter::TakeHit(int32 damage)
{
	ServerRPC_TakeDamage(damage);

}




// damage
void AAlienSwarmCharacter::ServerRPC_TakeDamage_Implementation(int32 damage)
{
	MultiRPC_TakeDamage(damage);
	

}
void AAlienSwarmCharacter::MultiRPC_TakeDamage_Implementation(int32 damage)
{

	HP -= damage;


	float hpratio = (float)HP / (float)MaxHP;

	testPC->SetHP(hpratio);


	UE_LOG(LogTemp, Warning, TEXT("PlayerTakeDamage"));
}

// 2�� ���� ��ü �� �ٸ� Ŭ���忡�� ���� �� ����
void AAlienSwarmCharacter::ServerRPC_SecondWeapon_Implementation()
{
	MultiRPC_SecondWeapon();

}
void AAlienSwarmCharacter::MultiRPC_SecondWeapon_Implementation()
{
	DetachWeapon(SubWeapon);
	DetachWeapon(Weapon);


	ChangeWeapon(Weapon2);
}
//////////////////////////////////////////////


// 1�� ���� ��ü �� �ٸ� Ŭ���忡�� ���� �� ����
void AAlienSwarmCharacter::ServerRPC_FirstWeapon_Implementation()
{
	MultiRPC_FirstWeapon();

}
void AAlienSwarmCharacter::MultiRPC_FirstWeapon_Implementation()
{
	DetachWeapon(SubWeapon);
	DetachWeapon(Weapon2);


	ChangeWeapon(Weapon);
}
//////////////////////////////////////////////


// ���� ���� ��ü �� �ٸ� Ŭ���忡�� ���� �� ����
void AAlienSwarmCharacter::ServerRPC_SubWeapon_Implementation()
{
	MultiRPC_SubWeapon();
}

void AAlienSwarmCharacter::MultiRPC_SubWeapon_Implementation()
{
	DetachWeapon(Weapon2);
	DetachWeapon(Weapon);


	ChangeWeapon(SubWeapon);
}
//////////////////////////////////////////////

void AAlienSwarmCharacter::ServerRPC_TurnPlayer_Implementation(FVector _mousePos, FRotator _turn)
{
	MultiRPC_TurnPlayer(_mousePos,_turn);
}

void AAlienSwarmCharacter::MultiRPC_TurnPlayer_Implementation(FVector _mousePos, FRotator _turn)
{
	DrawDebugSphere(GetWorld(), _mousePos, 50.0f, 3, FColor::Red, false, 0, 0, 1);

	// �÷��̾ trun �������� ȸ�� ��Ų��. 
	this->SetActorRotation(_turn);
	
}
void AAlienSwarmCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//rotYaw�� ���� �ֱ⸶�� �� Ŭ���̾�Ʈ�� �ѷ��� Ŭ���̾�Ʈ�� �������� ������,
	DOREPLIFETIME(AAlienSwarmCharacter, mousePos);
}