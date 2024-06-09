// Fill out your copyright notice in the Description page of Project Settings.


#include "StartActor.h"
#include "Components/BoxComponent.h"
#include "../AlienSwarmCharacter.h"
#include "Net/UnrealNetwork.h"
#include "TestPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

// Sets default values
AStartActor::AStartActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(rootComp);

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(rootComp);

}

// Called when the game starts or when spawned
void AStartActor::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AStartActor::StartOverlap);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &AStartActor::StartOverlapEnd);
}

// Called every frame
void AStartActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStartActor::StartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AAlienSwarmCharacter* playerREF = Cast<AAlienSwarmCharacter>(OtherActor);
	if (playerREF) {
		waitingPlayers++;
		UE_LOG(LogTemp, Warning, TEXT("On : %d"), waitingPlayers);
		int32 playerNum = 0;
		for (TActorIterator<AAlienSwarmCharacter> target(GetWorld()); target; ++target)
		{
			playerNum++;
		}
		UE_LOG(LogTemp, Warning, TEXT("PlayerNum : %d"), playerNum);
		if (waitingPlayers == playerNum) {
			ServerRPC_GameStart();
		}
	}
}

void AStartActor::StartOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AAlienSwarmCharacter* playerREF = Cast<AAlienSwarmCharacter>(OtherActor);
	if (playerREF) {
		waitingPlayers--;
		UE_LOG(LogTemp, Warning, TEXT("Out Cheaked: %d"), waitingPlayers);
	}
	UE_LOG(LogTemp, Warning, TEXT("Out : %d"), waitingPlayers);
}

void AStartActor::ServerRPC_GameStart_Implementation()
{
	GetWorld()->ServerTravel("/Game/Choi/Levels/TestLevel?listen");
}

