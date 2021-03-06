// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SphereComponent.h"
#include "DangerZone.h"

// Sets default values
ADangerZone::ADangerZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	Area = CreateDefaultSubobject<USphereComponent>(TEXT("Area"));
	Area->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADangerZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADangerZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

