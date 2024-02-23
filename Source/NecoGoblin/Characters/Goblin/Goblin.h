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
	float AttackDelay = 1.2f;
	class AGoblinGameMode* GameMode;
	volatile bool IsAttacking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	int32 HealthPickupSpawnRate = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	TSubclassOf<class AHealthPickup> HealthPickupClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FRotator HeadRotation;

	virtual void BeginPlay() override;

public:
	AGoblin();

	FORCEINLINE uint8 GetTeam() { return 2; };
	FORCEINLINE bool GetIsAttacking() { return IsAttacking; }
	FORCEINLINE void SetIsAttacking(bool isAttacking) { IsAttacking = isAttacking; }
	virtual bool CheckAlive() override;
	virtual void SetRunSpeed(float MovementSpeedModiifer);

	void OnHeadHit(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	void OnBodyHit(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
