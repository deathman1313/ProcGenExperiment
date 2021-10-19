// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "OrbitCam.h"

// Sets default values
AOrbitCam::AOrbitCam()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Arm1 = CreateDefaultSubobject<USpringArmComponent>(TEXT("Arm1"));
	Arm1->SetupAttachment(RootComponent);
	Arm1->bDoCollisionTest = false;
	Arm2->AddLocalRotation(FRotator(20.f, 0.f, 0.f));
	Arm1->TargetArmLength = 35000.f;

	Arm2 = CreateDefaultSubobject<USpringArmComponent>(TEXT("Arm2"));
	Arm2->SetupAttachment(Arm1);
	Arm2->AddLocalRotation(FRotator(180.f, 0.f, 0.f));
	Arm2->TargetArmLength = 0.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(Arm2);
	Camera->AddLocalRotation(FRotator(180.f, 0.f, 0.f));
}

// Called when the game starts or when spawned
void AOrbitCam::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOrbitCam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AOrbitCam::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

