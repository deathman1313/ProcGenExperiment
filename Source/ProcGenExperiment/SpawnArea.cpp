 // Fill out your copyright notice in the Description page of Project Settings.

#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameObject.h"
#include "Blueprint/UserWidget.h"
#include "Components/SphereComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "SpawnArea.h"

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

	UUserWidget* Test;
	Test = CreateWidget<UUserWidget>(GetWorld(), GenMenuClass, FName());
	Test->AddToViewport();
}

// Called every frame
void ASpawnArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnArea::SetRandomness()
{

	//UE_LOG(LogTemp, Warning, TEXT("%d"), Settings.Seed);
	// Sets random stream seed
	RandomGenerator = FRandomStream(Settings.Seed);

	// Destroys all currently spawned objects
	for (AGameObject* Obj : Objects)
	{
		Obj->Destroy();
	}
	Objects.Empty();

	// Spawns new objects
	SpawnObjects();
}

void ASpawnArea::SpawnObjects()
{
	for (int i = 0; i < Settings.Tier1Amt; i++)
	{
		int Insurance = 0;
		FTransform Position;
		bool bValidPosition = false;
		while (!bValidPosition)
		{
			Position = FindPosition();
			bValidPosition = IsPositionValid(Position, EObjectType::Tier1);
			Insurance++;
			if (Insurance > Settings.Attempts)
			{
				bValidPosition = true;
				UE_LOG(LogTemp, Warning, TEXT("Borked"));
			}
		}
		// Create Object
		AGameObject* NewObj = GetWorld()->SpawnActor<AGameObject>(ObjectToSpawn, Position.GetLocation(), Position.GetRotation().Rotator(), FActorSpawnParameters());
		NewObj->Type = EObjectType::Tier1;
		NewObj->Cube->SetMaterial(0, Materials[0]);
		Objects.Add(NewObj);
	}
	for (int i = 0; i < Settings.Tier2Amt; i++)
	{
		int Insurance = 0;
		FTransform Position;
		bool bValidPosition = false;
		while (!bValidPosition)
		{
			Position = FindPosition();
			bValidPosition = IsPositionValid(Position, EObjectType::Tier2);
			Insurance++;
			if (Insurance > Settings.Attempts)
			{
				bValidPosition = true;
				UE_LOG(LogTemp, Warning, TEXT("Borked"));
			}
		}
		// Create Object
		AGameObject* NewObj = GetWorld()->SpawnActor<AGameObject>(ObjectToSpawn, Position.GetLocation(), Position.GetRotation().Rotator(), FActorSpawnParameters());
		NewObj->Type = EObjectType::Tier2;
		NewObj->Cube->SetMaterial(0, Materials[1]);
		Objects.Add(NewObj);
	}
	for (int i = 0; i < Settings.Tier3Amt; i++)
	{
		int Insurance = 0;
		FTransform Position;
		bool bValidPosition = false;
		while (!bValidPosition)
		{
			Position = FindPosition();
			bValidPosition = IsPositionValid(Position, EObjectType::Tier3);
			Insurance++;
			if (Insurance > Settings.Attempts)
			{
				bValidPosition = true;
				UE_LOG(LogTemp, Warning, TEXT("Borked"));
			}
		}
		if (Insurance > Settings.Attempts)
		{
		}
		else
		{
			// Create Object
			AGameObject* NewObj = GetWorld()->SpawnActor<AGameObject>(ObjectToSpawn, Position.GetLocation(), Position.GetRotation().Rotator(), FActorSpawnParameters());
			NewObj->Type = EObjectType::Tier3;
			NewObj->Cube->SetMaterial(0, Materials[2]);
			Objects.Add(NewObj);
		}
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
	// Check if constraints are enabled
	if (Settings.bIsZoningEnabled)
	{
		if (!InZone(Position.GetLocation(), Type))
		{
			return(false);
		}
	}
	if (Settings.bIsDistancingEnabled)
	{
		if (!IsDistanced(Position.GetLocation()))
		{
			return(false);
		}
	}
	if (Settings.bIsDangerEnabled)
	{
		if (!NotInDanger(Position.GetLocation(), Type))
		{
			return(false);
		}
	}
	if (Settings.bIsSightEnabled)
	{
		// Get Array of View Points
		TArray<AActor*> ViewPoints;
		switch (Type)
		{
		case EObjectType::Tier1:
			ViewPoints.Add(PlayerStart);
		case EObjectType::Tier2:
			for (AGameObject* Obj : Objects)
			{
				if (Obj->Type == EObjectType::Tier1)
				{
					ViewPoints.Add(Obj);
				}
			}
		case EObjectType::Tier3:
			for (AGameObject* Obj : Objects)
			{
				if (Obj->Type == EObjectType::Tier2)
				{
					ViewPoints.Add(Obj);
				}
			}
		}
		if (!InSight(Position.GetLocation(), ViewPoints))
		{
			return(false);
		}
	}
	if (Settings.bIsNavigationEnabled)
	{
		// Get Array of Start Points
		TArray<AActor*> StartPoints;
		switch (Type)
		{
		case EObjectType::Tier1:
			StartPoints.Add(PlayerStart);
		case EObjectType::Tier2:
			for (AGameObject* Obj : Objects)
			{
				if (Obj->Type == EObjectType::Tier1)
				{
					StartPoints.Add(Obj);
				}
			}
		case EObjectType::Tier3:
			for (AGameObject* Obj : Objects)
			{
				if (Obj->Type == EObjectType::Tier2)
				{
					StartPoints.Add(Obj);
				}
			}
		}
		if (!IsNavigable(Position.GetLocation(), StartPoints))
		{
			return(false);
		}
	}
	// If all contraints are valid
	return(true);
}

bool ASpawnArea::InZone(FVector Location, EObjectType Type)
{
	float Distance = FVector::Dist(PlayerStart->GetActorLocation(), Location);

	switch (Type)
	{
	case EObjectType::Tier1:
		if (Distance >= Settings.Tier1ZoneMin && Distance <= Settings.Tier1ZoneMax)
		{
			return(true);
		} 
		else 
		{
			return(false);
		}
	case EObjectType::Tier2:
		if (Distance >= Settings.Tier2ZoneMin && Distance <= Settings.Tier2ZoneMax)
		{
			return(true);
		}
		else 
		{
			return(false);
		}
	case EObjectType::Tier3:
		if (Distance >= Settings.Tier3ZoneMin && Distance <= Settings.Tier3ZoneMax)
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
		if (FVector::Dist(Obj->GetActorLocation(), Location) <= Settings.Distance)
		{
			return(false);
		}
	}
	return(true);
}

bool ASpawnArea::NotInDanger(FVector Location, EObjectType Type)
{
	int maxDanger = 0;
	switch (Type)
	{
	case EObjectType::Tier1:
		maxDanger = 0;
	case EObjectType::Tier2:
		maxDanger = 1;
	case EObjectType::Tier3:
		maxDanger = 2;
	}
	for (ADangerZone* Zone : DangerZones) 
	{
		if (FVector::Dist(Zone->GetActorLocation(), Location) < Zone->Area->GetScaledSphereRadius() && Zone->DangerLevel > maxDanger)
		{
			return(false);
		}
	}
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
		if (!GetWorld()->LineTraceSingleByChannel(LandscapePointData, StartLocation, EndLocation, ECC_GameTraceChannel1))
		{
			return(true);
		}
	}
	return(false);
}

bool ASpawnArea::IsNavigable(FVector Location, TArray<AActor*> StartPoints)
{
	for (AActor* Actor : StartPoints)
	{
		UNavigationPath* Path;
		Path = UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), Actor->GetActorLocation(), Location);
		if (Path->IsValid())
		{
			if (!Path->IsPartial())
			{
				return(true);
			}
		}
	}
	return(false);
}