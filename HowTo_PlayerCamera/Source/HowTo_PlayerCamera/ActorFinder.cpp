// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorFinder.h"

// Sets default values
AActorFinder::AActorFinder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AActorFinder::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActorFinder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<FString> actors = GetActors();
}

TArray<FString> AActorFinder::GetActors()
{
	TArray<FString> actors;

	for (TObjectIterator<AActor> Itr; Itr; ++Itr)
	{
		if (Itr->IsA(AActor::StaticClass()))
		{
			actors.Add(Itr->GetActorLabel());
		}
	}
	return actors;
}