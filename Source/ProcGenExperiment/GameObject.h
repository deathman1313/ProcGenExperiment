// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameObject.generated.h"

UENUM(BlueprintType)
enum class EObjectType : uint8
{
	Tier1,
	Tier2,
	Tier3
};

UCLASS()
class PROCGENEXPERIMENT_API AGameObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameObject();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EObjectType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* Cube;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* ViewPoint;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
