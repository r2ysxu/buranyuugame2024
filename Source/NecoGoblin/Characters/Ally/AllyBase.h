// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Weapons/Ranged/FirearmWeapon.h"
#include "../../Weapons/Weapon.h"
#include "../Humanoid.h"
#include "AllyBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class NECOGOBLIN_API AAllyBase : public AHumanoid {
	GENERATED_BODY()

private:
	FTimerHandle OnFlinchHandler;

	void OnStopFlinching();

protected:
	bool IsFlinching = false;
	AFirearmWeapon* Firearm;
	
	virtual void BeginPlay() override;
	void SetupFirearmWeapon();

public:
	AAllyBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName FirearmSocketName = FName("hand_l");

	FORCEINLINE uint8 GetTeam() override { return 1; }
	virtual TSubclassOf<class AWeapon> GetFirearmWeaponClass() { return nullptr; }

	virtual void OnFireWeapon();
	virtual void HealthPot(float HealAmount);

	UFUNCTION()
	void OnHeadHit(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION()
	void OnBodyHit(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
