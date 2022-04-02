// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnFinder.h"

// Sets default values
APawnFinder::APawnFinder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APawnFinder::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APawnFinder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<APawn> pawns = GetPawns();
}

TArray<APawn> APawnFinder::GetPawns()
{
	TArray<APawn> pawns;

	for (TObjectIterator<APawn> Itr; Itr; ++Itr)
	{
		if (Itr->IsA(APawn::StaticClass()))
		{
			// pawns.Add(Itr);
		}
	}
	return pawns;
}