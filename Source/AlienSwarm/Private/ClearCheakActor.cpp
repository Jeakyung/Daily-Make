// Fill out your copyright notice in the Description page of Project Settings.


#include "ClearCheakActor.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "../AlienSwarmCharacter.h"
#include "TestPlayerController.h"
#include "AlienSwarmGameInstance.h"

// Sets default values
AClearCheakActor::AClearCheakActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(rootComp);

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	boxComp->SetupAttachment(rootComp);
	boxComp->SetBoxExtent(FVector(400.0f, 400.0f, 100.0f));

	clearText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Clear"));
	clearText->SetupAttachment(rootComp);
	clearText->SetRelativeRotation(FRotator(90.0f, 180.0f, 0.0f));
	clearText->SetWorldScale3D(FVector(10.0f));
	clearText->SetVisibility(false);
}

// Called when the game starts or when spawned
void AClearCheakActor::BeginPlay()
{
	Super::BeginPlay();
	
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AClearCheakActor::StartOverlap);
	boxComp->OnComponentEndOverlap.AddDynamic(this, &AClearCheakActor::StartOverlapEnd);

}

// Called every frame
void AClearCheakActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AClearCheakActor::StartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsClearReady) {
		AAlienSwarmCharacter* playerREF = Cast<AAlienSwarmCharacter>(OtherActor);
		if (playerREF) {
			//죽은 사람이 있을수도 있으니까 누가 올라설때마다 생존자 체크
			int32 playerNum = 0;
			for (TActorIterator<AAlienSwarmCharacter> target(GetWorld()); target; ++target)
			{
				playerNum++;
				if (target->HasAuthority()) {
					hostPC = Cast<ATestPlayerController>(target->GetController());
				}
			}
			waitingPlayers++;

			if (waitingPlayers == playerNum) {
				clearText->SetVisibility(true);
				FTimerHandle gameEndTimer;
				FTimerDelegate gameEndDelegate = FTimerDelegate::CreateUObject(this, &AClearCheakActor::GameEnd);
				if (!GetWorld()->GetTimerManager().IsTimerActive(gameEndTimer)) {

					GetWorld()->GetTimerManager().SetTimer(gameEndTimer, gameEndDelegate, 5.0f, false);
				}
			}
		}
	}
}

void AClearCheakActor::StartOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AAlienSwarmCharacter* playerREF = Cast<AAlienSwarmCharacter>(OtherActor);
	if (playerREF) {
		waitingPlayers--;
	}
}

void AClearCheakActor::GameEnd()
{
	if (HasAuthority()) {
		UE_LOG(LogTemp, Warning, TEXT("Exit Host"));
		ATestPlayerController* pc = Cast<ATestPlayerController>(GetWorld()->GetFirstPlayerController());
		pc->ClientReturnToMainMenuWithTextReason(NSLOCTEXT("HostOutMsg", "HostOut", "Host Exited!"));
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("TitleLevel"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Exit Client"));

		auto* gi = Cast<UAlienSwarmGameInstance>(GetWorld()->GetGameInstance());
		if (gi)
		{
			bool result = gi->sessionInterface->DestroySession(FName(*gi->mySessionName));

			if (result)
			{
				auto* pc = GetWorld()->GetFirstPlayerController();
				pc->ClientTravel("/Game/Net/Maps/LobbyMap", ETravelType::TRAVEL_Absolute);
			}
		}
	}
}

