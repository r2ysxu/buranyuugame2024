// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Humanoid.h"
#include "Goblin.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class NECOGOBLIN_API AGoblin : public AHumanoid {
	GENERATED_BODY()

protected:
	float AttackDelay = 3.f;
	class ANecoGoblinGameMode* GameMode;

	virtual void BeginPlay() override;

public:
	FORCEINLINE uint8 GetTeam() { return 2; };
	virtual void TakeHitDamage(float damage, AActor* DamageCauser) override;

	void OnHeadHit(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	void OnBodyHit(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
