// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeSkillComponent.h"
#include "MainCharacter.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

const static float RESERVE_AMMO_AMOUNT = 0.1f;
const static float FIRE_RATE_AMOUNT = 0.02f;
const static float RELOAD_SPEED_AMOUNT = 0.02f;
const static float HEADSHOT_DMG_AMOUNT = 0.1f;
const static float STAMINA_AMOUNT = 0.05f;
const static float STAMINA_REGEN_AMOUNT = 0.05f;
const static float MOVEMENT_SPEED_AMOUNT = 0.1f;
const static float DODGE_CHANCE_AMOUNT = 0.1f;
const static float ADDITIONAL_HP_AMOUNT = 15.f;
const static float ADDITIONAL_HEAL_AMOUNT = 30.f;
const static float REGEN_HP_AMOUNT = 0.01f;

// Sets default values for this component's properties
UUpgradeSkillComponent::UUpgradeSkillComponent() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UUpgradeSkillComponent::SetParentCharacter(AMainCharacter* character) {
	ParentCharacter = character;
}

void UUpgradeSkillComponent::SetupWidget(UUserWidget* skillHudWidget) {
	SkillHudWidget = skillHudWidget;
	if (SkillHudWidget) {
		SkillHudWidget->AddToViewport();
		SkillHudWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

// Called when the game starts
void UUpgradeSkillComponent::BeginPlay() {
	Super::BeginPlay();
}

void UUpgradeSkillComponent::ResetSkills() {
	ReserveAmmoModifier = 1.f;
	FireRateModifier = 1.f;
	ReloadSpeedModifier = 1.f;
	HeadshotDmgModfiier = 1.f;
	StaminaModifier = 1.f;
	StaminaRegenModifier = 1.f;
	MovementSpeedModifier = 1.f;
	DodgeChanceModifier = 1.f;
	AdditionalHP = 0.f;
	AdditionalHeal = 0.f;
	RegenHP = 0.f;
}

bool UUpgradeSkillComponent::AddSkillPoint(FNecoSkills Skill) {
	if (SkillPoints < 1 || Skills[(uint8)Skill] > 4) return false;
	switch (Skill) {
	case FNecoSkills::VE_ReserveAmmo:
		ReserveAmmoModifier += RESERVE_AMMO_AMOUNT;
		break;
	case FNecoSkills::VE_FireRate:
		if (Skills[(uint8)FNecoSkills::VE_ReserveAmmo] < 1) return false;
		FireRateModifier += FIRE_RATE_AMOUNT;
		break;
	case FNecoSkills::VE_ReloadSpeed:
		if (Skills[(uint8)FNecoSkills::VE_FireRate] < 1 || Skills[(uint8)FNecoSkills::VE_StaminaRegen] < 1) return false;
		ReloadSpeedModifier -= RELOAD_SPEED_AMOUNT;
		break;
	case FNecoSkills::VE_Headshot:
		if (Skills[(uint8)FNecoSkills::VE_FireRate] < 1) return false;
		HeadshotDmgModfiier += HEADSHOT_DMG_AMOUNT;
		break;
	case FNecoSkills::VE_Stamina:
		StaminaModifier += STAMINA_AMOUNT;
		break;
	case FNecoSkills::VE_StaminaRegen:
		if (Skills[(uint8)FNecoSkills::VE_Stamina] < 1) return false;
		StaminaRegenModifier += STAMINA_REGEN_AMOUNT;
		break;
	case FNecoSkills::VE_MovementSpeed:
		if (Skills[(uint8)FNecoSkills::VE_MovementSpeed] < 1) return false;
		MovementSpeedModifier += MOVEMENT_SPEED_AMOUNT;
		ParentCharacter->SetRunSpeed(MovementSpeedModifier);
		break;
	case FNecoSkills::VE_ChangeDodge:
		if (Skills[(uint8)FNecoSkills::VE_StaminaRegen] < 1 || Skills[(uint8)FNecoSkills::VE_ChangeDodge] < 1) return false;
		DodgeChanceModifier += DODGE_CHANCE_AMOUNT;
		break;
	case FNecoSkills::VE_MaxHP:
		AdditionalHP += ADDITIONAL_HP_AMOUNT;
		ParentCharacter->AddMaxHP(ADDITIONAL_HP_AMOUNT);
		break;
	case FNecoSkills::VE_HealHP:
		if (Skills[(uint8)FNecoSkills::VE_MaxHP] < 1) return false;
		AdditionalHeal += ADDITIONAL_HEAL_AMOUNT;
		break;
	case FNecoSkills::VE_RegenHP:
		if (Skills[(uint8)FNecoSkills::VE_RegenHP] < 1) return false;
		RegenHP += REGEN_HP_AMOUNT;
		break;
	}
	SkillPoints--;
	Skills[(uint8)Skill] = Skills[(uint8)Skill] + 1;
	return true;
}

void UUpgradeSkillComponent::EnterScreen() {
	APlayerController* controller = Cast<APlayerController>(ParentCharacter->GetController());
	controller->bShowMouseCursor = true;
	SkillHudWidget->SetVisibility(ESlateVisibility::Visible);
	controller->SetInputMode(FInputModeGameAndUI());
	controller->SetIgnoreMoveInput(true);
	controller->SetIgnoreLookInput(true);
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.1f);
}

void UUpgradeSkillComponent::ExitScreen() {
	APlayerController* controller = Cast<APlayerController>(ParentCharacter->GetController());
	controller->bShowMouseCursor = false;
	SkillHudWidget->SetVisibility(ESlateVisibility::Hidden);
	controller->SetInputMode(FInputModeGameOnly());
	controller->SetIgnoreLookInput(false);
	controller->SetIgnoreMoveInput(false);
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
}

int UUpgradeSkillComponent::GetSkillLevel(FNecoSkills Skill) {
	return Skills[(uint8)Skill];
}

bool UUpgradeSkillComponent::AddExpPoints(int points) {
	if (SkillPoints >= MAX_SKILL_POINTS) return false;
	ExpPoints += points;
	if (ExpPoints > GetExpPointToLevel()) {
		SkillPoints++;
		ExpPoints -= GetExpPointToLevel();
	}
	return true;
}
