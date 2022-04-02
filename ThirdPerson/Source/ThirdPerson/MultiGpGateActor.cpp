// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiGpGateActor.h"

// Sets default values
AMultiGpGateActor::AMultiGpGateActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SuperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MY SUPER MESH"));

}

// Called when the game starts or when spawned
void AMultiGpGateActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMultiGpGateActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

