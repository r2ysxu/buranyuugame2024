// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../../Weapons/Melee/MeleeWeapon.h"
#include "../../Weapons/Weapon.h"
#include "Goblin.h"
#include "MeleeGoblinController.h"
#include "MeleeGoblinCharacter.generated.h"

UCLASS()
class NECOGOBLIN_API AMeleeGoblinCharacter : public AGoblin {
	GENERATED_BODY()

private:
	FTimerHandle InitateAttackHandler;
	FTimerHandle OnMeleeAttackHandler;

protected:

	AMeleeGoblinController* AiController;
	AMeleeWeapon* MeleeWeapon;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void SetupWeapon();
	void LookAtTarget_Implementation(FRotator Rotation);

public:
	// Sets default values for this character's properties
	AMeleeGoblinCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class AWeapon> MeleeWeaponClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class UBoxComponent* MeleeDetectionBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class UAnimMontage* MeleeAttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName WeaponSocketName = FName("weapon_r");

	void LookAtTarget(FRotator Rotation);
	UFUNCTION(NetMulticast, Reliable) void Multicast_LookAtTarget(FRotator Rotation);

	UFUNCTION()
	void OnWithinMeleeAttackRange(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOutsideMeleeAttackRange(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

	void SetRunSpeed(float MovementSpeedModifier) override;
	void InitiateMeleeAttack();
	void OnAttackStop();
	void TakeHitDamage(float damage, AActor* DamageCauser) override;
	bool CheckAlive() override;
	void OnDecompose() override;

};
