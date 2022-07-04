// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Spaceship.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = Cast<ASpaceship>(UGameplayStatics::GetPlayerPawn(this,0));
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Super::Tick(DeltaTime);
	AddActorLocalOffset(FVector(1 * Speed, 0, 0));
	if (GetActorLocation().X >= 400.f)
	{
		Deactivate();
	}
}

void ABullet::Deactivate()
{
	SetActorTickEnabled(false);
	SetActorLocation(FVector(0.f, 0.f, -1500.f));
	PlayerRef->BulletPool.Emplace(this);
}
void ABullet::Activate()
{
	SetActorTickEnabled(true);
	float InitialPosition = PlayerRef->ShipMesh->GetComponentLocation().Y;
	SetActorLocation(FVector(-350.f, InitialPosition, 142.f));
}

