// Fill out your copyright notice in the Description page of Project Settings.


#include "Asteroid.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterGM.h"
#include "Spaceship.h"
#include "Bullet.h"



// Sets default values
AAsteroid::AAsteroid()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void AAsteroid::BeginPlay()
{
	Super::BeginPlay();

	GameModeRef = Cast<AShooterGM>(UGameplayStatics::GetGameMode(this));
	PlayerRef = Cast<ASpaceship>(UGameplayStatics::GetPlayerPawn(this, 0));

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AAsteroid::OnOverlapBegin);
}

// Called every frame
void AAsteroid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorLocalOffset(FVector(-1 * Speed, 0, 0));
	if (GetActorLocation().X <= -400.f)
	{
		Deactivate();
	}

}

void AAsteroid::Deactivate()
{
	//return to pool
	SetActorTickEnabled(false);
	SetActorLocation(FVector(0.f,0.f,-1000.f));
	GameModeRef->AsteroidPool.Emplace(this);
}

void AAsteroid::Activate()
{
	SetActorTickEnabled(true);
	float InitialPosition = FMath::RandRange(-400.f, 400.f);
	SetActorLocation(FVector(400.f, InitialPosition, 142.f));
	SetResistance();

}

void AAsteroid::SetResistance()
{
	//random resistance when pulling out of the pool
	Resistance = FMath::RandRange(50.f, 100.f);
	InitialResistance = Resistance;
	float ScaleToSet = FMath::GetMappedRangeValueClamped(FVector2D(50.f,100.f), FVector2D(0.5f, 1.5f), Resistance);
	SetActorScale3D(FVector(ScaleToSet, ScaleToSet, ScaleToSet));
}

void AAsteroid::OnOverlapBegin(class UPrimitiveComponent* Comp, AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABullet* Bullet = Cast<ABullet>(OtherActor);
		if (Bullet)
		{
			Resistance = FMath::Clamp(Resistance - Bullet->Power,0.f, 200.f);
			PlayerRef->UpdateHUD();
			if (Resistance == 0)
			{
				PlayerRef->Score += InitialResistance;
				Deactivate();
			}
			Bullet->Deactivate();
			return;
		}
}