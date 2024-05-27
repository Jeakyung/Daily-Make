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
	// 카메라 최초 위치
	cameraLoc = FollowCamera->GetRelativeLocation();
	
			
	// 총 액터 생성 후 플레이어에게 Attach
	if (WeaponClass) 
	{		
		// 총 액터 생성하기
		Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass);
		
		// 만약 생성이 유효하다면
		if (nullptr != Weapon)
		{
			// 총을 플레이어의 손에 부착
			Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("RightHandSocket"));
			Weapon->SetActorRelativeLocation(FVector(4.7f, -11, 1.8f));
			Weapon->SetActorRelativeRotation(FRotator(-11, 81, -81));
			Weapon->SetActorRelativeScale3D(FVector(1.25f));
			Weapon->Equip(this);
			UE_LOG(LogTemp, Warning, TEXT("spawnWeapon"));
		}
	}

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
		if (nullptr != Weapon) {
			Weapon->OnFire(mousePos);
			PlayFireMontage();
		}
	}
}

void AAlienSwarmCharacter::OnIAReload(const FInputActionValue& Value)
{
	PlayReloadMontage();
	
}

void AAlienSwarmCharacter::TurnPlayer()
{
	if (nullptr != Controller)
	{
		

		FVector mouseLocation, mouseDirection;
		mouseDirection.Normalize();
		PlayerController = GetWorld()->GetFirstPlayerController();
		// 플레이어의 마우스 위치와 방향 값을 각각 mouseLocation과 mouseDirection에 넣어준다.
		PlayerController->DeprojectMousePositionToWorld(mouseLocation, mouseDirection);
	
		// 라인 그리기
		// start : 카메라 위치
		FVector start = FollowCamera->GetComponentLocation();
		// end = 화면상에 마우스가 가르키는 위치 
		FVector end = start + mouseDirection * 100000;
		// 플레이어와의 충돌 무시
		FCollisionQueryParams params;
		FHitResult hitResult;
		params.AddIgnoredActor(this);

		// 부딪힌 것이 있는지 판별하는 변수 (일단 만들어놓음)
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility, params);
		
		// 방향 = 마우스 위치에 쏘여진 라인트레이스와 충돌한 위치 - 플레이어 위치
		mousePos = hitResult.ImpactPoint;
		FVector direction = mousePos - GetActorLocation();
		FRotator turnDir = direction.Rotation();
 		FRotator trun = FRotator(0, turnDir.Yaw+10, 0);
		
		DrawDebugSphere(GetWorld(), mousePos,50.0f,3,FColor::Red,false,0,0,1);

		

		// 플레이어를 trun 방향으로 회전 시킨다. 
		this->SetActorRotation(trun);

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

void AAlienSwarmCharacter::OnMyReloadFinished()
{
	Weapon->OnReload();
	bReloading = false;
	UE_LOG(LogTemp, Warning, TEXT("Reload"));
}
