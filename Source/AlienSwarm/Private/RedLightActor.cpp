// Fill out your copyright notice in the Description page of Project Settings.


#include "RedLightActor.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/SpotLightComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/RotatingMovementComponent.h>

// Sets default values
ARedLightActor::ARedLightActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(rootComp);
	rootComp->SetVisibility(false);

	redLight1 = CreateDefaultSubobject<USpotLightComponent>(TEXT("RedLight1"));
	redLight1->SetupAttachment(rootComp);
	redLight1->LightColor = FColor::Red;
	redLight1->Intensity = 20000.0f;
	redLight1->OuterConeAngle = 70.0f;
	redLight1->SetVisibility(false);

	redLight2 = CreateDefaultSubobject<USpotLightComponent>(TEXT("RedLight2"));
	redLight2->SetupAttachment(rootComp);
	redLight2->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	redLight2->LightColor = FColor::Red;
	redLight2->Intensity = 20000.0f;
	redLight2->OuterConeAngle = 70.0f;
	redLight2->SetVisibility(false);

	rotateComp = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotateComp"));
	rotateComp->RotationRate = FRotator(0.0f, 180.0f, 0.0f);

	bReplicates = true;
}

// Called when the game starts or when spawned
void ARedLightActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARedLightActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARedLightActor::ServerRPC_LightOn_Implementation()
{
	MulitRPC_LightOn();
}

void ARedLightActor::MulitRPC_LightOn_Implementation()
{
	rootComp->SetVisibility(true);
	redLight1->SetVisibility(true);
	redLight2->SetVisibility(true);
}

