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

	SetRandomness(1);
}

// Called every frame
void ASpawnArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnArea::SetRandomness(int Seed)
{
	// Sets random stream seed
	RandomGenerator = FRandomStream(Seed);

	// Destroys all currently spawned objects
	for (int i = 0; i < Objects.Num(); i++)
	{
		Objects[i]->Destroy();
	}
	Objects.Empty();

	// Spawns new objects
	for (int i = 0; i < 5; i++)
	{
		FTransform Position;
		bool bValidPosition = false;
		while (!bValidPosition)
		{
			Position = FindPosition();
			bValidPosition = IsPositionValid(Position, EObjectType::Tier1);
		}
		AGameObject* Test = GetWorld()->SpawnActor<AGameObject>(ObjectToSpawn, Position.GetLocation(), Position.GetRotation().Rotator(), FActorSpawnParameters());
	}
}

FTransform ASpawnArea::FindPosition() 
{
	//Find random location in 2D space
	float PosX = RandomGenerator.FRandRange(Area->GetRelativeLocation().X - Area->GetScaledBoxExtent().X, Area->GetRelativeLocation().X + Area->GetScaledBoxExtent().X);
	float PosY = RandomGenerator.FRandRange(Area->GetRelativeLocation().Y - Area->GetScaledBoxExtent().Y, Area->GetRelativeLocation().Y + Area->GetScaledBoxExtent().Y);

	//Find location on surface
	FHitResult LandscapePointData;
	GetWorld()->LineTraceSingleByChannel(LandscapePointData, FVector(PosX, PosY, Area->GetRelativeLocation().Z + Area->GetScaledBoxExtent().Z), FVector(PosX, PosY, Area->GetRelativeLocation().Z - Area->GetScaledBoxExtent().Z), ECC_WorldStatic);
	
	// Calaculate Rotator
	FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(FVector(0.f, 0.f, 0.f), LandscapePointData.Normal);
	return(FTransform(FRotator(Rotation.Pitch - 90.f, Rotation.Yaw, Rotation.Roll), FVector(PosX, PosY, LandscapePointData.Location.Z)));
}

bool ASpawnArea::IsPositionValid(FTransform Position, EObjectType Type)
{
	return(InZone(Position.GetLocation(), Type));
}

bool ASpawnArea::InZone(FVector Location, EObjectType Type)
{
	float Distance = FVector::Dist(PlayerStart->GetActorLocation(), Location);

	switch (Type)
	{
	case EObjectType::Tier1:
		if (Distance < 1000)
		{
			return(true);
		} 
		else 
		{
			return(false);
		}
	case EObjectType::Tier2:
		if (Distance > 1000 && Distance < 2000)
		{
			return(true);
		}
		else 
		{
			return(false);
		}
	case EObjectType::Tier3:
		if (Distance > 2000 && Distance < 3000)
		{
			return(true);
		}
		else
		{
			return(false);
		}
	default:
		return(true);
	}
}

bool ASpawnArea::IsNavigable(TArray<AActor*> StartPoints, FVector Location)
{
	for (int i = 0; i < StartPoints.Num(); i++)
	{
		UNavigationPath* Test;
		Test = UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), StartPoints[i]->GetActorLocation(), Location, NavMesh);
	}
	return(true);
}