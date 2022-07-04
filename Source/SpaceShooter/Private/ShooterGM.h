// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShooterGM.generated.h"

class AAsteroid;
class APickableObject;

UCLASS()
class AShooterGM : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
private:
	void CreatePools();
	void SpawnObject();
public:
	TArray<AAsteroid*> AsteroidPool;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AAsteroid> AsteroidClass;
	int32 AsteroidPoolSize = 10;
	UPROPERTY(EditAnywhere)
	float TimerInterval = 1.5f;

	TArray<APickableObject*> PickablePool;
	UPROPERTY(EditAnywhere)
	TSubclassOf<APickableObject> PickableClass;
	int32 PickablePoolSize = 5;
};
