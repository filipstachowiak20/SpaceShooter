// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickableObject.generated.h"

class UStaticMeshComponent;
class UTextRenderComponent;
class AShooterGM;

UCLASS()
class SPACESHOOTER_API APickableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickableObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UTextRenderComponent* RenderText;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Deactivate();
	void Activate();
	void SetMode();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateColor();

	UPROPERTY(BlueprintReadOnly)
	int32 Mode = 0;

	float Speed = 3.f;

	AShooterGM* GameModeRef;
};
