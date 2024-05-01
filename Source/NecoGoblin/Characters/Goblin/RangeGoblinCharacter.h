// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Goblin.h"

#include "CoreMinimal.h"
#include "RangeGoblinCharacter.generated.h"

/**
 * 
 */
UCLASS()
class NECOGOBLIN_API ARangeGoblinCharacter : public AGoblin {
	GENERATED_BODY()
	
private:
	const FName MainPlayer = FName("MainPlayer");

	FTimerHandle AttackCooldownHandler;
	FTimerHandle AttackResetHandler;
	bool IsAttackCooldown = false;

	void LookAtTarget_Implementation(FVector Targetlocation);
	void InitiateRangeAttack_Implementation(FVector OutTossVelocity);

protected:
	class AThrownWeapon* Weapon;
	float AttackCooldownRate = 2.f;

	virtual void BeginPlay() override;
	void SetupWeapon();

public:
	ARangeGoblinCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		TSubclassOf<class AThrownWeapon> RangeWeaponClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		FName WeaponSocketName = FName("weapon_r");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		class UAnimMontage* RangeAttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		float TossVelocity = 2000.f;

	void LookAtTarget(FVector Targetlocation);
	UFUNCTION(NetMulticast, Reliable) void Multicast_LookAtTarget(FVector Targetlocation);
	void InitiateRangeAttack(FVector OutTossVelocity);
	UFUNCTION(NetMulticast, Reliable) void Multicast_InitiateRangeAttack(FVector OutTossVelocity);

	void SetRunSpeed(float MovementSpeedModifier) override;
	bool CheckRangeAttack(class AAllyBase* TargetCharacter, FVector& OutTossVelocity);
	void TrackTargetStopMovement(class AAllyBase* TargetCharacter);
	void OnAttackReset();
	void OnAttackCooldownReset();
	bool CheckAlive() override;
	void OnDecompose() override;
};
