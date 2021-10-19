// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameObject.h"
#include "SpawnArea.h"
#include "NavigationSystem.h"

// Sets default values
ASpawnArea::ASpawnArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	Area = CreateDefaultSubobject<UBoxComponent>(TEXT("Area"));
	Area->SetupAttachment(RootComponent);
	Area->SetGenerateOverlapEvents(false);
}

// Called when the game starts or when spawned
void ASpawnArea::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < 5; i++)
	{
		FTransform Position = FindPosition();
		AGameObject* Test = GetWorld()->SpawnActor<AGameObject>(ObjectToSpawn, Position.GetLocation(), Position.GetRotation().Rotator(), FActorSpawnParameters());
	}
}

// Called every frame
void ASpawnArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FTransform ASpawnArea::FindPosition() 
{
	//Find random location in 2D space
	float PosX = UKismetMathLibrary::RandomFloatInRange(Area->GetRelativeLocation().X - Area->GetScaledBoxExtent().X, Area->GetRelativeLocation().X + Area->GetScaledBoxExtent().X);
	float PosY = UKismetMathLibrary::RandomFloatInRange(Area->GetRelativeLocation().Y - Area->GetScaledBoxExtent().Y, Area->GetRelativeLocation().Y + Area->GetScaledBoxExtent().Y);

	//Find location on surface
	FHitResult LandscapePointData;
	GetWorld()->LineTraceSingleByChannel(LandscapePointData, FVector(PosX, PosY, Area->GetRelativeLocation().Z + Area->GetScaledBoxExtent().Z), FVector(PosX, PosY, Area->GetRelativeLocation().Z - Area->GetScaledBoxExtent().Z), ECC_WorldStatic);
	
	// Calaculate Rotator
	FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(FVector(0.f, 0.f, 0.f), LandscapePointData.Normal);
	return(FTransform(FRotator(Rotation.Pitch - 90.f, Rotation.Yaw, Rotation.Roll), FVector(PosX, PosY, LandscapePointData.Location.Z)));
}

bool ASpawnArea::IsNavigable() 
{
	UNavigationPath* Test;
	Test = UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), FVector(), FVector(), NavMesh);
	return(true);
}