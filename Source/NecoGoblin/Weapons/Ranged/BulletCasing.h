// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletCasing.generated.h"

UCLASS()
class NECOGOBLIN_API ABulletCasing : public AActor {
	GENERATED_BODY()

private:
	FTimerHandle FlightHandler;

protected:
	UPROPERTY(EditAnywhere, Category = Mesh)
	class UStaticMesh* Mesh;

	class UStaticMeshComponent* MeshComponent;
	class UProjectileMovementComponent* ProjectileMovement;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION() void DestoryAfterSeconds();

public:	
	ABulletCasing();
};
