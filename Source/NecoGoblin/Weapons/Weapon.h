// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Characters/Humanoid.h"
#include "Weapon.generated.h"

UCLASS(Abstract)
class NECOGOBLIN_API AWeapon : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AWeapon();

protected:
	class UBoxComponent* MeleeAttackWeaponBox;
	AHumanoid* Wielder;
	bool MeleeAttacking = false;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	void AttachToWielder(AHumanoid* WieldCharacter, FName SocketName);
	virtual void EquipWeapon(FName SocketName) {}
	virtual uint8 GetWeaponTeam() { return 0; }
	virtual float GetWeaponDamage() { return 0.f; }
};
