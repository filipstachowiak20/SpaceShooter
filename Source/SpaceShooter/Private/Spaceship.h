// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Spaceship.generated.h"

class UCameraComponent;
class USceneComponent;
class UStaticMeshComponent;
class UParticleSystem;
class UParticleSystemComponent;
class ABullet;

UCLASS()
class ASpaceship : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASpaceship();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* SceneComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* SceneCompShip;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ShipMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* FireParticle;

private:
	void Move(float Value);
	void Fire();
	void StopFire();

	void SpawnBullet();
	void CreateBulletPool();

	void UpdateFire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateHUD();

	void OnDeath();
	void Revive();
	void SetCollision();
private:
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* Comp, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FTimerHandle FireHandle;

	float TimerInterval = 0.24f;
public:

	TArray<ABullet*> BulletPool;
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABullet> BulletClass;
	int32 BulletPoolSize = 40;
	UPROPERTY(EditAnywhere)
		UParticleSystem* ExplosionParticle;

	UPROPERTY(EditAnywhere)
	float Speed = 5.f;
	UPROPERTY(EditAnywhere)
	float MapWidth = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ForceField = 200.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Score = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Ammo = 0.f;

	bool SpecialWeapon = false;

};
