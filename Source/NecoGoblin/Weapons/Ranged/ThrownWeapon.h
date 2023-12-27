// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Weapon.h"
#include "ThrownWeapon.generated.h"

/**
 * 
 */
UCLASS()
class NECOGOBLIN_API AThrownWeapon : public AWeapon {
	GENERATED_BODY()

private:
	bool IsInFlight = false;
	FTimerHandle OnThrowWeaponHandler;
	FTimerHandle FlightResetHandler;

	void ThrowWeapon();

protected:
	UPROPERTY(EditAnywhere, Category = Mesh)
	class UStaticMesh* Mesh;
	class UStaticMeshComponent* WeaponMeshComponent;

	class UProjectileMovementComponent* ProjectileMovement;

	virtual void BeginPlay() override;

	UFUNCTION()	void OnHitBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	AThrownWeapon();

	void EquipWeapon(FName SocketName) override;
	float GetWeaponDamage() override;
	uint8 GetWeaponTeam() override;
	UFUNCTION() bool OnFire(FVector TossVelocity, float delay);
	void OnResetProjectile();
};
