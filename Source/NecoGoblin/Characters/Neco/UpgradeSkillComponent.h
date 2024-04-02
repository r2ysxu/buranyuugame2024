// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UpgradeSkillComponent.generated.h"

UENUM(BlueprintType)
enum class FNecoSkills : uint8 {
	VE_ReserveAmmo		UMETA(DisplayName = "Upgrade Reserve Ammo"),
	VE_FireRate			UMETA(DisplayName = "Upgrade Fire Rate"),
	VE_ReloadSpeed		UMETA(DisplayName = "Upgrade Reload Speed"),
	VE_Headshot			UMETA(Displayname = "Upgrade Headshot Damage"),
	VE_Stamina			UMETA(Displayname = "Upgrade Stamina"),
	VE_StaminaRegen		UMETA(Displayname = "Upgrade Stamina Regen"),
	VE_MovementSpeed	UMETA(Displayname = "Upgrade Movement Speed"),
	VE_WeaponDamage		UMETA(Displayname = "Upgrade Damage"),
	VE_MaxHP			UMETA(Displayname = "Upgrade Max HP"),
	VE_HealHP			UMETA(Displayname = "Upgrade Heal Pot"),
	VE_RegenHP			UMETA(Displayname = "Upgrade HP Regen"),
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NECOGOBLIN_API UUpgradeSkillComponent : public UActorComponent {
	GENERATED_BODY()

private:
	const static size_t SKILLS_SIZE = 11;
	const static size_t MAX_SKILL_POINTS = SKILLS_SIZE * 5;
	const static int EXP_PER_LEVEL = 8;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	class USkillsMenuWidget* SkillHudWidget;

	class AMainCharacter* ParentCharacter;
	int AddedSkillPoints = 0;
	int SkillPoints = 15;
	int ExpPoints = 0;

	uint8 Skills[SKILLS_SIZE];

	float ReserveAmmoModifier = 1.f;
	float FireRateModifier = 1.f;
	float ReloadSpeedModifier = 1.f;
	float HeadshotDmgModfiier = 1.f;
	float StaminaModifier = 1.f;
	float StaminaRegenModifier = 1.f;
	float MovementSpeedModifier = 1.f;
	float WeaponDamageModifier = 1.f;
	float AdditionalHP = 0.f;
	float AdditionalHeal = 0.f;
	float RegenHP = 0.f;

public:	

	// Sets default values for this component's properties
	UUpgradeSkillComponent();

	void SetParentCharacter(AMainCharacter* mainCharacter);
	void SetupWidget(class USkillsMenuWidget* skillHudWidget);
	void ResetSkills();

	UFUNCTION(BlueprintCallable) bool AddSkillPoint(FNecoSkills Skill);
	UFUNCTION(BlueprintCallable) void EnterScreen();
	UFUNCTION(BlueprintCallable) void ExitScreen();
	UFUNCTION(BlueprintCallable) FORCEINLINE int GetExpPointToLevel() { return EXP_PER_LEVEL; }
	UFUNCTION(BlueprintCallable) FORCEINLINE int GetExpPoints() { return ExpPoints; }
	UFUNCTION(BlueprintCallable) FORCEINLINE int GetSkillPoints() { return SkillPoints; }
	UFUNCTION(BlueprintCallable) int GetSkillLevel(FNecoSkills Skill);

	FORCEINLINE float GetAdditionalReserveAmmoModifier() { return ReserveAmmoModifier; }
	FORCEINLINE float GetFireRateModifier() { return FireRateModifier; }
	FORCEINLINE float GetReloadSpeedModifier() { return ReloadSpeedModifier; }
	FORCEINLINE float GetHeadshotModifier() { return HeadshotDmgModfiier; }
	FORCEINLINE float GetStaminaModifier() { return StaminaModifier; }
	FORCEINLINE float GetStaminaRegenModifier() { return StaminaRegenModifier; }
	FORCEINLINE float GetMovementSpeedModifer() { return MovementSpeedModifier; }
	FORCEINLINE float GetWeaponDamageModifier() { return WeaponDamageModifier; }
	FORCEINLINE float GetAdditionalHP() { return AdditionalHP; }
	FORCEINLINE float GetAdditionalHeal() { return AdditionalHeal; }
	FORCEINLINE float GetRegenHP() { return RegenHP; }

	bool AddExpPoints(int points);
};
