// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Weapon.h"
#include "../../Characters/Humanoid.h"
#include "MeleeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class NECOGOBLIN_API AMeleeWeapon : public AWeapon {
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, Category = Mesh)
	class UStaticMesh* Mesh;
	class UStaticMeshComponent* WeaponMeshComponent;

	AHumanoid* LastHitCharacter;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	AMeleeWeapon();
	void EquipWeapon(FName SocketName) override;
	float GetWeaponDamage() override;
	uint8 GetWeaponTeam() override;

	void SetIsMeleeAttacking(bool IsMeleeAttacking);
	FORCEINLINE bool GetIsMeleeAttacking() { return MeleeAttacking; }
	FORCEINLINE AHumanoid* GetLastHitCharacter() { return LastHitCharacter; }
	FORCEINLINE void SetLastHitCharacter(AHumanoid* actor) { LastHitCharacter = actor; }
};
