// Fill out your copyright notice in the Description page of Project Settings.
// 대기방에서 나갈때 사용하는 액터

#include "EXITActor.h"
#include "Components/BoxComponent.h"
#include "../AlienSwarmCharacter.h"
#include "Net/UnrealNetwork.h"
#include "TestPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "AlienSwarmGameInstance.h"

// Sets default values
AEXITActor::AEXITActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(rootComp);

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(rootComp);


}

// Called when the game starts or when spawned
void AEXITActor::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AEXITActor::EXIT);
}

// Called every frame
void AEXITActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEXITActor::EXIT(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AAlienSwarmCharacter* playerREF = Cast<AAlienSwarmCharacter>(OtherActor);
	if (playerREF) {
		if (HasAuthority()) {
			UE_LOG(LogTemp, Warning, TEXT("Exit Host"));
			ATestPlayerController* pc = Cast<ATestPlayerController>(playerREF->Controller);
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
}

