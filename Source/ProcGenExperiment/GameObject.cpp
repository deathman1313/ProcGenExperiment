// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SceneComponent.h"
#include "GameObject.h"

// Sets default values
AGameObject::AGameObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));

	ViewPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ViewPoint"));
	ViewPoint->ComponentTags.Add(FName("ViewPoint"));
}

// Called when the game starts or when spawned
void AGameObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGameObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

