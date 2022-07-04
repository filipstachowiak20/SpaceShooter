// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableObject.h"
#include "Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterGM.h"

// Sets default values
APickableObject::APickableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
	RenderText = CreateDefaultSubobject<UTextRenderComponent>("RenderText");
	RenderText->SetupAttachment(Mesh);

}

// Called when the game starts or when spawned
void APickableObject::BeginPlay()
{
	Super::BeginPlay();

	GameModeRef = Cast<AShooterGM>(UGameplayStatics::GetGameMode(this));
	
}

// Called every frame
void APickableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorLocalOffset(FVector(-1 * Speed, 0, 0));
	if (GetActorLocation().X <= -400.f)
	{
		Deactivate();
	}

}

void APickableObject::Deactivate()
{
	//return to pool
	SetActorTickEnabled(false);
	SetActorLocation(FVector(0.f, 0.f, -1000.f));
	GameModeRef->PickablePool.Emplace(this);
}

void APickableObject::Activate()
{
	Mode = FMath::RandRange(0, 2);
	SetMode();
	SetActorTickEnabled(true);
	float InitialPosition = FMath::RandRange(-400.f, 400.f);
	SetActorLocation(FVector(400.f, InitialPosition, 142.f));

}

void APickableObject::SetMode()
{
	//visual representation of the current mode(health or weapon)
	UpdateColor();
	if (Mode == 0)
	{
		RenderText->SetText(FText::FromString(TEXT("Health")));
		return;
	}
	if (Mode == 1)
	{
		RenderText->SetText(FText::FromString(TEXT("Rifle 1")));
		return;
	}
	if (Mode == 2)
	{
		RenderText->SetText(FText::FromString(TEXT("Rifle 2")));
		return;
	}
	
}
