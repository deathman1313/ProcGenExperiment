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
	for (AGameObject* Obj : Objects)
	{
		Obj->Destroy();
	}
	Objects.Empty();

	// Spawns new objects
	for (int i = 0; i < 5; i++)
	{
		int Insurance = 0;
		FTransform Position;
		bool bValidPosition = false;
		while (!bValidPosition)
		{
			Position = FindPosition();
			bValidPosition = IsPositionValid(Position, EObjectType::Tier1);
			Insurance++;
			if (Insurance > 100)
			{
				bValidPosition = true;
				UE_LOG(LogTemp, Warning, TEXT("Borked"));
			}
		}
		AGameObject* NewObj = GetWorld()->SpawnActor<AGameObject>(ObjectToSpawn, Position.GetLocation(), Position.GetRotation().Rotator(), FActorSpawnParameters());
		Objects.Add(NewObj);
	}
}

FTransform ASpawnArea::FindPosition() 
{
	//Find random location in 2D space
	float PosX = RandomGenerator.FRandRange(Area->GetRelativeLocation().X - Area->GetScaledBoxExtent().X, Area->GetRelativeLocation().X + Area->GetScaledBoxExtent().X);
	float PosY = RandomGenerator.FRandRange(Area->GetRelativeLocation().Y - Area->GetScaledBoxExtent().Y, Area->GetRelativeLocation().Y + Area->GetScaledBoxExtent().Y);

	//Find location on surface
	FHitResult LandscapePointData;
	GetWorld()->LineTraceSingleByChannel(LandscapePointData, FVector(PosX, PosY, Area->GetRelativeLocation().Z + Area->GetScaledBoxExtent().Z), FVector(PosX, PosY, Area->GetRelativeLocation().Z - Area->GetScaledBoxExtent().Z), ECC_GameTraceChannel1);
	
	// Calaculate Rotator
	FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(FVector(0.f, 0.f, 0.f), LandscapePointData.Normal);
	return(FTransform(FRotator(Rotation.Pitch - 90.f, Rotation.Yaw, Rotation.Roll), FVector(PosX, PosY, LandscapePointData.Location.Z)));
}

bool ASpawnArea::IsPositionValid(FTransform Position, EObjectType Type)
{
	// This is for testing
	TArray<AActor*> Temp;
	Temp.Add(PlayerStart);
	/*
	for (AGameObject* Obj : Objects)
	{
		Temp.Add(Obj);
	}
	*/

	// This is also just for testing
	return(InZone(Position.GetLocation(), Type) && IsDistanced(Position.GetLocation()) && InSight(Position.GetLocation(), Temp));
}

bool ASpawnArea::InZone(FVector Location, EObjectType Type)
{
	float Distance = FVector::Dist(PlayerStart->GetActorLocation(), Location);

	switch (Type)
	{
	case EObjectType::Tier1:
		if (Distance < 7500)
		{
			return(true);
		} 
		else 
		{
			return(false);
		}
	case EObjectType::Tier2:
		if (Distance > 7500 && Distance < 15000)
		{
			return(true);
		}
		else 
		{
			return(false);
		}
	case EObjectType::Tier3:
		if (Distance > 15000 && Distance < 25000)
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

bool ASpawnArea::IsDistanced(FVector Location) 
{
	for (AGameObject* Obj : Objects)
	{
		if (FVector::Dist(Obj->GetActorLocation(), Location) <= 2500)
		{
			return(false);
		}
	}
	return(true);
}

bool ASpawnArea::InDanger(FVector Location, EObjectType Type)
{
	return(true);
}

bool ASpawnArea::InSight(FVector Location, TArray<AActor*> ViewPoints)
{
	FVector StartLocation;

	for (AActor* Actor : ViewPoints)
	{
		// Check for View Point
		TArray<UActorComponent*> Components = Actor->GetComponentsByTag(USceneComponent::StaticClass(), FName("ViewPoint"));
		if (Components[0])
		{
			if (Components[0]->IsA<USceneComponent>())
			{
				// Find location of View Point
				StartLocation = CastChecked<USceneComponent>(Components[0])->GetComponentLocation();
			}
			// If no View Point found, use actor location
			else
			{
				StartLocation = Actor->GetActorLocation();
			}
		}
		else
		{
			StartLocation = Actor->GetActorLocation();
		}
		// Check line of sight
		FHitResult LandscapePointData;
		FVector EndLocation = (UKismetMathLibrary::GetDirectionUnitVector(Location, StartLocation) * 5) + Location;
		return(!GetWorld()->LineTraceSingleByChannel(LandscapePointData, StartLocation, EndLocation, ECC_GameTraceChannel1));
	}
	return(true);
}

bool ASpawnArea::IsNavigable(FVector Location, TArray<AActor*> StartPoints)
{
	for (AActor* Actor : StartPoints)
	{
		UNavigationPath* Test;
		Test = UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), Actor->GetActorLocation(), Location, NavMesh);
	}
	return(true);
}