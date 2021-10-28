// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerSpawnLocation.h"
#include "GameObject.h"
#include "DangerZone.h"
#include "SpawnArea.generated.h"

USTRUCT(BlueprintType)
struct FGenerationSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Seed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Tier1Amt = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Tier1ZoneMin = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Tier1ZoneMax = 7500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Tier2Amt = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Tier2ZoneMin = 7500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Tier2ZoneMax = 15000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Tier3Amt = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Tier3ZoneMin = 15000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Tier3ZoneMax = 25000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsZoningEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsDistancingEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsDangerEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsSightEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsNavigationEnabled;
};


UCLASS()
class PROCGENEXPERIMENT_API ASpawnArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnArea();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* Area;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRandomStream RandomGenerator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UUserWidget> GenMenuClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGenerationSettings Settings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		APlayerSpawnLocation* PlayerStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AGameObject*> Objects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AGameObject> ObjectToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<ADangerZone*> DangerZones;

	UFUNCTION(BlueprintCallable)
		void SetRandomness();

	UFUNCTION(BlueprintCallable)
		void SpawnObjects();

	UFUNCTION(BlueprintCallable)
		FTransform FindPosition();

	UFUNCTION(BlueprintCallable)
		bool IsPositionValid(FTransform Position, EObjectType Type);

	UFUNCTION(BlueprintCallable)
		bool InZone(FVector Location, EObjectType Type);

	UFUNCTION(BlueprintCallable)
		bool IsDistanced(FVector Location);

	UFUNCTION(BlueprintCallable)
		bool InDanger(FVector Location, EObjectType Type);

	UFUNCTION(BlueprintCallable)
		bool InSight(FVector Location, TArray<AActor*> ViewPoints);

	UFUNCTION(BlueprintCallable)
		bool IsNavigable(FVector Location, TArray<AActor*> StartPoints);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
