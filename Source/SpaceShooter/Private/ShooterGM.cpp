// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGM.h"
#include "Asteroid.h"
#include "PickableObject.h"

void AShooterGM::BeginPlay()
{
	Super::BeginPlay();

	CreatePools();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AShooterGM::SpawnObject, TimerInterval, true);

}

void AShooterGM::CreatePools()
{
	//Asteroids and pickable objects(health, weapon) are puuled from object pools
	FActorSpawnParameters Params;
	for (int32 i = 0; i < AsteroidPoolSize; i++)
	{
		AAsteroid* Asteroid = GetWorld()->SpawnActor<AAsteroid>(AsteroidClass,FVector(0.f,0.f,-1000.f),FRotator(0.f,0.f,0.f), Params);
		AsteroidPool.Emplace(Asteroid);
		Asteroid->Deactivate();
	}
	FActorSpawnParameters Params2;
	for (int32 i = 0; i < PickablePoolSize; i++)
	{
		APickableObject* Pickable = GetWorld()->SpawnActor<APickableObject>(PickableClass, FVector(0.f, 0.f, -1000.f), FRotator(0.f, 0.f, 0.f), Params2);
		PickablePool.Emplace(Pickable);
		Pickable->Deactivate();
	}
}

void AShooterGM::SpawnObject()
{
	//Get objects from pools randomly. 90% chance of asteroid, 10% of sth else
	int32 RandInt =  FMath::RandRange(0,9);
	if (RandInt < 9)
	{
		if (AsteroidPool.Num() > 0)
		{
			AsteroidPool[0]->Activate();
			AAsteroid* AsteroidToRemove = AsteroidPool[0];
			AsteroidPool.Remove(AsteroidToRemove);
		}
	}
	else
	{
		if (PickablePool.Num() > 0)
		{
			PickablePool[0]->Activate();
			APickableObject* PickableToRemove = PickablePool[0];
			PickablePool.Remove(PickableToRemove);
		}
	}
}