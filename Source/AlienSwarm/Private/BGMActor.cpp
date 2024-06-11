// Fill out your copyright notice in the Description page of Project Settings.


#include "BGMActor.h"
#include "Components/AudioComponent.h"

// Sets default values
ABGMActor::ABGMActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(rootComp);

	beforeBGM = CreateDefaultSubobject<UAudioComponent>(TEXT("beforeBGM"));
	beforeBGM->SetupAttachment(rootComp);
	beforeBGM->SetAutoActivate(true);

	AfterBGM = CreateDefaultSubobject<UAudioComponent>(TEXT("AfterBGM"));
	AfterBGM->SetupAttachment(rootComp);
	AfterBGM->SetAutoActivate(false);
}

// Called when the game starts or when spawned
void ABGMActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABGMActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABGMActor::ServerRPC_PlayAfter_Implementation()
{
	MultiRPC_PlayAfter();
}

void ABGMActor::MultiRPC_PlayAfter_Implementation()
{
	beforeBGM->Stop();
	AfterBGM->Play();
}

