// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Weapons/Ranged/FirearmWeapon.h"
#include "../../Weapons/Weapon.h"
#include "../Humanoid.h"
#include "NecoSpirit.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class NECOGOBLIN_API ANecoSpirit : public AHumanoid {
	GENERATED_BODY()

private:

protected:
	AFirearmWeapon* Firearm;
	
	virtual void BeginPlay() override;
	void SetupFirearmWeapon();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName FirearmSocketName = FName("hand_l");

	FORCEINLINE uint8 GetTeam() override { return 1; }
	virtual TSubclassOf<class AWeapon> GetFirearmWeaponClass() { return nullptr; }

	virtual void OnFireWeapon();

	UFUNCTION()
	void OnHeadHit(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION()
	void OnBodyHit(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
