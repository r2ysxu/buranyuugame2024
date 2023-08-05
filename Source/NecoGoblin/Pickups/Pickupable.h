// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Characters/Neco/NecoSpirit.h"
#include "Pickupable.generated.h"

UCLASS()
class NECOGOBLIN_API APickupable : public AActor {
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, Category = Mesh)
	class UStaticMesh* Mesh;
	class UStaticMeshComponent* PickupMeshComponent;
	class USphereComponent* PickupRangeBox;

public:	
	// Sets default values for this actor's properties
	APickupable();

	virtual void OnPickup(ANecoSpirit* actor) {}

	UFUNCTION()	void OnWithinPickupRange(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
