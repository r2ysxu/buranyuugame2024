// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Goblin.h"
#include "../Neco/NecoSpirit.h"

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

protected:
	class ARangeGoblinController* AiController;
	class AThrownWeapon* Weapon;
	float AttackCooldownRate = 3.f;

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
		float TossVelocity = 1000.f;

	FORCEINLINE class ARangeGoblinController* GetAIController() { return AiController; }
	FORCEINLINE void SetAIController(class ARangeGoblinController* controller) { AiController = controller; }

	void SetRunSpeed(float MovementSpeedModifier) override;
	bool CheckRangeAttack(ANecoSpirit* TargetCharacter, FVector& OutTossVelocity);
	void InitiateRangeAttack(FVector& OutTossVelocity);
	void OnAttackReset();
	void OnAttackCooldownReset();
	bool CheckAlive() override;
	void OnDecompose() override;
};
