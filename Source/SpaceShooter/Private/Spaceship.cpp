// Fill out your copyright notice in the Description page of Project Settings.


#include "Spaceship.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Asteroid.h"
#include "Bullet.h"
#include "PickableObject.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASpaceship::ASpaceship()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//setting up components
	SceneComp = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	Camera = CreateDefaultSubobject<UCameraComponent>("ShipCamera");
	SceneCompShip = CreateDefaultSubobject<USceneComponent>("ShipScene");
	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>("ShipMesh");
	FireParticle = CreateDefaultSubobject<UParticleSystemComponent>("Fire");

	//Components hierarchy
	SceneComp->SetupAttachment(RootComponent);
	SceneCompShip->SetupAttachment(SceneComp);
	Camera->SetupAttachment(SceneComp);
	ShipMesh->SetupAttachment(SceneCompShip);
	FireParticle->SetupAttachment(SceneCompShip);

	Camera->SetRelativeLocation(FVector(0.f, 0.f, 1000.f));
	Camera->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));

}

// Called when the game starts or when spawned
void ASpaceship::BeginPlay()
{
	Super::BeginPlay();

	ShipMesh->OnComponentBeginOverlap.AddDynamic(this, &ASpaceship::OnOverlapBegin);
	CreateBulletPool();
	UpdateHUD();
	UpdateFire();
}

// Called every frame
void ASpaceship::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASpaceship::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("Move", this, &ASpaceship::Move);
	InputComponent->BindAction("Fire", IE_Pressed, this, &ASpaceship::Fire);
	InputComponent->BindAction("Fire", IE_Released, this, &ASpaceship::StopFire);

}

void ASpaceship::Move(float Value)
{
	FVector Direction = GetActorRightVector();
	float PlayerLocation = SceneCompShip->GetRelativeLocation().Y;

	if ((Value > 0 && PlayerLocation < MapWidth) || (Value < 0 && PlayerLocation > -1 * MapWidth))//prevents falling off the restricted area
	{
		SceneCompShip->AddRelativeLocation(Direction * Value * Speed);
	}
}

void ASpaceship::Fire()
{
	if (!FireHandle.IsValid())
	{	//start firing if not firing already
		GetWorld()->GetTimerManager().SetTimer(FireHandle, this, &ASpaceship::SpawnBullet, TimerInterval, true);
	}
}

void ASpaceship::StopFire()
{	//stop firing
	GetWorld()->GetTimerManager().ClearTimer(FireHandle);
}



void ASpaceship::OnOverlapBegin(class UPrimitiveComponent* Comp, AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AAsteroid* Asteroid = Cast<AAsteroid>(OtherActor);

	if (Asteroid)
	{	//if asteroid hits player - subtract force field, die if force field == 0
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, Asteroid->GetActorTransform(), true);
		Asteroid->Deactivate();
		ForceField = FMath::Clamp(ForceField - Asteroid->InitialResistance, 0.f, 200.f);
		UpdateFire();
		if (ForceField <= 0)
		{
			OnDeath();
		}
		UpdateHUD();

		return;
	}
	APickableObject* Pickable = Cast<APickableObject>(OtherActor);
	if (Pickable)
	{
		Pickable->Deactivate();
		//Add ammo or force field according to the object mode variable
		int32 Mode = Pickable->Mode;
		if (Mode == 0)
		{
			ForceField = 200.f;
			UpdateFire();
		}
		if (Mode == 1)
		{
			SpecialWeapon = false;
			Ammo = FMath::RandRange(50, 100);
		}
		if (Mode == 2)
		{
			SpecialWeapon = true;
			Ammo = FMath::RandRange(50, 100);
		}
		UpdateHUD();
		return;
	}
}

void ASpaceship::SpawnBullet()
{ 
	//pull bullet actor from the pool
	if (Ammo == 0)
	{
		if (BulletPool.Num() > 0)
		{
			BulletPool[0]->Activate();
			BulletPool[0]->Power = 10;
			ABullet* BulletToRemove = BulletPool[0];
			BulletPool.Remove(BulletToRemove);
		}
		return;
	}
	if (!SpecialWeapon)
	{
		for(int32 i = 0;i<2;i++)
		if (BulletPool.Num() > 0)
		{
			BulletPool[0]->Activate();
			BulletPool[0]->AddActorLocalOffset(FVector(0,-20+i*40,0));
			BulletPool[0]->Power = 15;
			ABullet* BulletToRemove = BulletPool[0];
			BulletPool.Remove(BulletToRemove);
			Ammo = FMath::Clamp(Ammo -1,0,100);
		}
		UpdateHUD();
	}
	if (SpecialWeapon)
	{
		for (int32 i = 0; i < 4; i++)
		{
			if (BulletPool.Num() > 0)
			{
				BulletPool[0]->Activate();
				BulletPool[0]->Power = 20;
				ABullet* BulletToRemove = BulletPool[0];
				BulletPool[0]->AddActorLocalOffset(FVector(0, -60 + i * 40, 0));
				BulletPool.Remove(BulletToRemove);
				Ammo = FMath::Clamp(Ammo - 1, 0, 100);
			}
		}
		UpdateHUD();
	}
}

void ASpaceship::CreateBulletPool()
{
	//create pool object of bullets istead of spawning and destroying
	FActorSpawnParameters Params;
	for (int32 i = 0; i < BulletPoolSize; i++)
	{
		ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(BulletClass, FVector(0.f, 0.f, -1500.f), FRotator(0.f, 0.f, 0.f), Params);
		BulletPool.Emplace(Bullet);
		Bullet->Deactivate();
	}
}

void ASpaceship::OnDeath()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	FTimerHandle ReviveTimer;
	GetWorld()->GetTimerManager().SetTimer(ReviveTimer, this, &ASpaceship::Revive, 2.f, false);
}

void ASpaceship::Revive()
{
	SetActorHiddenInGame(false);
	FTimerHandle CollisionTimer;
	GetWorld()->GetTimerManager().SetTimer(CollisionTimer, this, &ASpaceship::SetCollision, 1.5f, false);//Allows player to get off the asteroid course when respawned

	ForceField = 200;
	Score = 0;
	Ammo = 0;
	SpecialWeapon = false;
	UpdateFire();
	UpdateHUD();
}

void ASpaceship::SetCollision()
{
	SetActorEnableCollision(true);
}

void ASpaceship::UpdateFire()
{
	//updates the fire particle according to current force field level
	if (ForceField <= 50)
	{
		FireParticle->SetHiddenInGame(false);
	}
	else
	{
		FireParticle->SetHiddenInGame(true);
	}
}