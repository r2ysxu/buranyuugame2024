// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Weapon.h"
#include "NiagaraComponent.h"
#include "FirearmWeapon.generated.h"

/**
 * 
 */
UCLASS()
class NECOGOBLIN_API AFirearmWeapon : public AWeapon
{
	GENERATED_BODY()

private:
	bool FireWeapon(FVector startLocation, FVector forwardVector, FCollisionQueryParams collisionParams, FHitResult& OutResult);

protected:

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	class UNiagaraSystem* BloodHitFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	class USkeletalMesh* Mesh;
	class USkeletalMeshComponent* WeaponMeshComponent;
	FTimerHandle InitiateFireHandler;
	volatile bool IsFiring = false;

	float MaxRange = 5000.f;
	float FireRate = 1.f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void WeaponFireStart();
	void WeaponFireStop();

public:
	AFirearmWeapon();
	void EquipWeapon(FName SocketName) override;
	float GetWeaponDamage() override;
	uint8 GetWeaponTeam() override;
	
	bool OnFire(FVector startLocation, FVector forwardVector, FCollisionQueryParams collisionParams, FHitResult &OutResult);
};
