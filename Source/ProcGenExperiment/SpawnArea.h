// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerSpawnLocation.h"
#include "GameObject.h"
#include "SpawnArea.generated.h"

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
	APlayerSpawnLocation* PlayerStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AGameObject> ObjectToSpawn;

	UFUNCTION(BlueprintCallable)
	FTransform FindPosition();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};