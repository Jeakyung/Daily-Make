// Fill out your copyright notice in the Description page of Project Settings.


#include "WarningTriggerActor.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Engine/DirectionalLight.h>
#include "EngineUtils.h"
#include "RedLightActor.h"
#include "DoorActor.h"
#include "EnemySpawner.h"
#include "ClearCheakActor.h"
#include "../AlienSwarmCharacter.h"

// Sets default values
AWarningTriggerActor::AWarningTriggerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(rootComp);

	triggerComp = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerComp"));
	triggerComp->SetupAttachment(rootComp);

	bReplicates = true;
}

// Called when the game starts or when spawned
void AWarningTriggerActor::BeginPlay()
{
	Super::BeginPlay();
	
	triggerComp->OnComponentBeginOverlap.AddDynamic(this, &AWarningTriggerActor::ActiveWarning);

	redLights.Empty();
	doors.Empty();
	spawners.Empty();

	for (TActorIterator<ARedLightActor> it(GetWorld()); it; ++it) {
		redLights.Add(*it);
	}
	for (TActorIterator<ADoorActor> it(GetWorld()); it; ++it) {
		doors.Add(*it);
	}
	for (TActorIterator<AEnemySpawner> it(GetWorld()); it; ++it) {
		spawners.Add(*it);
	}
}

// Called every frame
void AWarningTriggerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWarningTriggerActor::ActiveWarning(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AAlienSwarmCharacter* playerREF = Cast<AAlienSwarmCharacter>(OtherActor);
	if (playerREF && HasAuthority()) {
		SetOwner(playerREF->GetController());
		UE_LOG(LogTemp, Warning, TEXT("111111"));
		ServerRPC_ActiveWarning();
	}
}

void AWarningTriggerActor::ServerRPC_ActiveWarning_Implementation()
{
	MultiRPC_ActiveWarning();
}

void AWarningTriggerActor::MultiRPC_ActiveWarning_Implementation()
{
	if(dirLight) {
		ADirectionalLight* dLight = Cast<ADirectionalLight>(dirLight);
		if(dLight) {
			dLight->SetLightColor(FColor(150,150,150));
		}
	}

	if (clearCheak) {
		clearCheak->bIsClearReady = true;
	}

	for(int32 i = 0; i < redLights.Num(); i++) {
		redLights[i]->ServerRPC_LightOn();
	}

	for (int32 i = 0; i < doors.Num(); i++) {
		doors[i]->bIsLocked = true;
	}

	for (int32 i = 0; i < spawners.Num(); i++) {
		spawners[i]->bSpawnInfinity = true;
	}
}

