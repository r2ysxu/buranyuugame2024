// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickupable.h"
#include "HealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class NECOGOBLIN_API AHealthPickup : public APickupable
{
	GENERATED_BODY()

private:
	FTimerHandle OnDecomposeHandler;
	float DECOMPOSE_DELAY = 5.f;

public:
	virtual void BeginPlay() override;
	virtual void OnPickup(ANecoSpirit* actor) override;
	void OnDecompose();
};
