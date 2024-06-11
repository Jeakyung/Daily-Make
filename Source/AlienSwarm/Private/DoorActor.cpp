// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorActor.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
#include "../AlienSwarmCharacter.h"
#include "HitInterface.h"
#include "ToolEngineering.h"
#include "AlienEnemy.h"

// Sets default values
ADoorActor::ADoorActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(rootComp);

	doorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
	doorFrame->SetupAttachment(rootComp);

	door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	door->SetupAttachment(rootComp);

	openColl = CreateDefaultSubobject<UBoxComponent>(TEXT("OpenColl"));
	openColl->SetupAttachment(rootComp);

	lockBox = CreateDefaultSubobject<UBoxComponent>(TEXT("lockBox"));
	lockBox->SetupAttachment(door);
	lockBox->SetBoxExtent(FVector(10.0f, 70.0f, 120.0f));
	lockBox->SetRelativeLocation(FVector(0.0f, 0.0f, 126.0f));
	lockBox->SetCollisionProfileName(TEXT("NoCollision"));

	bReplicates = true;
}

// Called when the game starts or when spawned
void ADoorActor::BeginPlay()
{
	Super::BeginPlay();

	openColl->OnComponentBeginOverlap.AddDynamic(this, &ADoorActor::DoorOpen);
	openColl->OnComponentEndOverlap.AddDynamic(this, &ADoorActor::DoorClose);

	// openColl->OnComponentBeginOverlap.AddDynamic(this, &ADoorActor::EnemyOverlapDoorClosed);

	doorClosePos = door->GetRelativeLocation();
	doorOpenPos = doorClosePos - FVector(0.0f, 142.0f, 0.0f);
	doorPos = doorClosePos;
}

// Called every frame
void ADoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsDestroyed) {

		if (openstart) {
			doorTimer = FMath::Clamp(doorTimer + DeltaTime, 0.0f, doorOpenTime);
			doorPos = FMath::Lerp(doorClosePos, doorOpenPos, doorTimer / doorOpenTime);
			if (doorTimer == doorOpenTime) {
				openstart = false;
				bIsOpened = true;
			}
		}
		else if (closestart) {
			doorTimer = FMath::Clamp(doorTimer - DeltaTime, 0.0f, doorOpenTime);
			doorPos = FMath::Lerp(doorClosePos, doorOpenPos, doorTimer / doorOpenTime);
			if (doorTimer == 0.0f) {
				closestart = false;
				bIsOpened = false;
			}
		}
	}
	else {
		doorPos = doorOpenPos;
	}

	door->SetRelativeLocation(doorPos);

	
	if (doorHP <= 0)
	{
		bIsLocked = false;
		lockBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// openColl->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		door->SetVisibility(false);
		bIsDestroyed = true;
		bIsOpened = true;

		// UE_LOG(LogTemp, Warning, TEXT("enemyListNum: %d"), enemyList.Num());

 		// enemy->bIsTargetDoor = false;

		for (int32 i = 0; i < enemyList.Num(); i++)
		{
			enemyList[i]->bIsTargetDoor = false;
		}
		
	}
	
}

//void ADoorActor::EnemyAttackDoor()
//{
//	
//}

void ADoorActor::DoorOpen(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AAlienSwarmCharacter* playerREF = Cast<AAlienSwarmCharacter>(OtherActor);
	AAlienEnemy* enemy = Cast<AAlienEnemy>(OtherActor);

	if (playerREF)
	{
		SetOwner(GetWorld()->GetFirstPlayerController()->GetPawn());
	}
	
	else if(enemy) 
	{
		enemyList.Add(enemy);

		if (bIsLocked && !enemy->bAttackedBack)
		{
			if (doorHP > 0)
			{
				enemy->bIsTargetDoor = true;
				enemy->myDoorTarget = this;

				UE_LOG(LogTemp, Warning, TEXT("mytargetisdoor"));
			}
			else
			{
				enemy->bIsTargetDoor = false;

				bIsOpened = true;
				bIsLocked = false;

			}
		}
		else {
		
			enemy->TargetCheck();
			//enemy->bMoveAnim = true;
			//enemy->bAttackAnim = false;
		}

	}

	if (openstart || bIsOpened || bIsLocked) {
		return;
	}
	else {
		ServerRPC_DoorOpen();
	}


}

void ADoorActor::ServerRPC_DoorOpen_Implementation()
{
	closestart = false;
	openstart = true;
}

void ADoorActor::DoorClose(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AAlienSwarmCharacter* playerREF = Cast<AAlienSwarmCharacter>(OtherActor);
	//AAlienEnemy* enemy = Cast<AAlienEnemy>(OtherActor);

	if (playerREF) {
		SetOwner(GetWorld()->GetFirstPlayerController()->GetPawn());
	}

	TArray<AActor*> overlapList;
	openColl->GetOverlappingActors(overlapList);

	if (overlapList.Num() > 1) {
		return;
	}

	ServerRPC_DoorClose();
}

void ADoorActor::ServerRPC_DoorClose_Implementation()
{
	openstart = false;
	closestart = true;
}

void ADoorActor::DoorLock()
{
	SetOwner(GetWorld()->GetFirstPlayerController()->GetPawn());
	ServerRPC_DoorLock();
}

void ADoorActor::ServerRPC_DoorLock_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("serverlock"));
	bIsLocked = true;
	OnRep_bIsLocked();
}

void ADoorActor::DoorUnLock()
{
	SetOwner(GetWorld()->GetFirstPlayerController()->GetPawn());
	ServerRPC_DoorUnLock();
}

void ADoorActor::OnRep_HPCheck()
{	
	UE_LOG(LogTemp, Warning, TEXT("hp changed!!!"));
}

void ADoorActor::TakeHit(int32 damage)
{
	doorHP-=damage;
	UE_LOG(LogTemp, Warning, TEXT("doorHP: %d"), doorHP);

	
}

//void ADoorActor::EnemyOverlapDoorClosed(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	AAlienEnemy* enemy = Cast<AAlienEnemy>(OtherActor);
//
//	if (enemy)
//	{
//		if (bIsLocked && !enemy->bAttackedBack)
//		{
//			if (doorHP > 0)
//			{
//				enemy->bIsTargetDoor = true;
//				// enemy->bDoorOpen = false;
//
//				// 이동 멈추고 문 공격 동작
//				//enemy->AIEnemyController->StopMovement();
//
//				enemy->bMoveAnim = false;
//				enemy->bAttackAnim = true;
//
//				TakeHit(enemy->enemyCP);
//
//			}
//			else
//			{
//				FTimerHandle hnd;
//				GetWorld()->GetTimerManager().SetTimer(hnd, [&]() {
//					enemy->bIsTargetDoor = false;
//				}, 2.f, false);
//				enemy->bIsTargetDoor = false;
//				bIsOpened = true;
//				bIsLocked = false;
//			}
//		}
//		else {
//			enemy->bDoorOpen = true;
//
//			enemy->bMoveAnim = true;
//			enemy->bAttackAnim = false;
//		}
//	}
//}

void ADoorActor::ServerRPC_DoorUnLock_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("serverunlock"));
	bIsLocked = false;
	OnRep_bIsLocked();
}

void ADoorActor::OnRep_bIsLocked()
{
	UE_LOG(LogTemp, Warning, TEXT("onrep"));
	if (bIsLocked)
	{
		lockBox->SetCollisionProfileName(TEXT("BlockAll"));
	}
	else {
		lockBox->SetCollisionProfileName(TEXT("NoCollision"));
	}
}

void ADoorActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//rotYaw를 일정 주기마다 각 클라이언트에 뿌려서 클라이언트의 변수값을 고찬다,
	DOREPLIFETIME(ADoorActor, doorPos);
	DOREPLIFETIME(ADoorActor, bIsLocked);
	DOREPLIFETIME(ADoorActor, doorHP);
}// Fill out your copyright notice in the Description page of Project Settings.

