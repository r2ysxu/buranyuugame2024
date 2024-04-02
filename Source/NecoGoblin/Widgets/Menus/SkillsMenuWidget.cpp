// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillsMenuWidget.h"
#include "../../Characters/Neco/MainCharacter.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

int USkillsMenuWidget::OnAddSkillPoint(enum FNecoSkills Skill) {
	if (IsValid(Character)) {
		Character->UpgradeSkill(Skill);
		SkillPointsPrompt->SetText(FText::FromString(FString::FromInt(
			Character->GetSkillUpgrades()->GetSkillPoints()
		)));
		return Character->GetSkillUpgrades()->GetSkillLevel(Skill);
	}
	return 0;
}

int USkillsMenuWidget::GetSkillPoints(FNecoSkills Skill) {
	if (IsValid(Character)) {
		return Character->GetSkillUpgrades()->GetSkillLevel(Skill);
	}
	return 0;
}

void USkillsMenuWidget::NativeConstruct() {
	Skill_STR_01->OnClicked.AddDynamic(this, &USkillsMenuWidget::OnAddSTR01);
	Skill_STR_02->OnClicked.AddDynamic(this, &USkillsMenuWidget::OnAddSTR02);
	Skill_STR_03->OnClicked.AddDynamic(this, &USkillsMenuWidget::OnAddSTR03);
	Skill_DEX_01->OnClicked.AddDynamic(this, &USkillsMenuWidget::OnAddDEX01);
	Skill_DEX_02->OnClicked.AddDynamic(this, &USkillsMenuWidget::OnAddDEX02);
	Skill_DEX_03->OnClicked.AddDynamic(this, &USkillsMenuWidget::OnAddDEX03);
	Skill_CON_01->OnClicked.AddDynamic(this, &USkillsMenuWidget::OnAddCON01);
	Skill_CON_02->OnClicked.AddDynamic(this, &USkillsMenuWidget::OnAddCON02);
	Skill_CON_03->OnClicked.AddDynamic(this, &USkillsMenuWidget::OnAddCON03);
	Skill_STR_DEX_01->OnClicked.AddDynamic(this, &USkillsMenuWidget::OnAddSTR_DEX01);
	Skill_DEX_CON_01->OnClicked.AddDynamic(this, &USkillsMenuWidget::OnAddDEX_CON01);

	Skill_STR_01->OnHovered.AddDynamic(this, &USkillsMenuWidget::OnInfoTextSTR01);
	Skill_STR_02->OnHovered.AddDynamic(this, &USkillsMenuWidget::OnInfoTextSTR02);
	Skill_STR_03->OnHovered.AddDynamic(this, &USkillsMenuWidget::OnInfoTextSTR03);
	Skill_DEX_01->OnHovered.AddDynamic(this, &USkillsMenuWidget::OnInfoTextDEX01);
	Skill_DEX_02->OnHovered.AddDynamic(this, &USkillsMenuWidget::OnInfoTextDEX02);
	Skill_DEX_03->OnHovered.AddDynamic(this, &USkillsMenuWidget::OnInfoTextDEX03);
	Skill_CON_01->OnHovered.AddDynamic(this, &USkillsMenuWidget::OnInfoTextCON01);
	Skill_CON_02->OnHovered.AddDynamic(this, &USkillsMenuWidget::OnInfoTextCON02);
	Skill_CON_03->OnHovered.AddDynamic(this, &USkillsMenuWidget::OnInfoTextCON03);
	Skill_STR_DEX_01->OnHovered.AddDynamic(this, &USkillsMenuWidget::OnInfoTextSTR_DEX01);
	Skill_DEX_CON_01->OnHovered.AddDynamic(this, &USkillsMenuWidget::OnInfoTextDEX_CON01);

	STR02_pts->SetIsEnabled(false);
	STR03_pts->SetIsEnabled(false);
	DEX02_pts->SetIsEnabled(false);
	DEX03_pts->SetIsEnabled(false);
	CON02_pts->SetIsEnabled(false);
	CON03_pts->SetIsEnabled(false);
	DEX_CON_01_pts->SetIsEnabled(false);
	STR_DEX_01_pts->SetIsEnabled(false);
}

void USkillsMenuWidget::OnAddSTR01() {
	int skillPoints = OnAddSkillPoint(FNecoSkills::VE_ReserveAmmo);
	STR01_pts->SetText(FText::FromString(FString::FromInt(skillPoints)));
	if (skillPoints >= MAX_SKILL) STR02_pts->SetIsEnabled(true);
}

void USkillsMenuWidget::OnAddSTR02() {
	int skillPoints = OnAddSkillPoint(FNecoSkills::VE_FireRate);
	STR02_pts->SetText(FText::FromString(FString::FromInt(skillPoints)));
	if (skillPoints >= MAX_SKILL) {
		STR03_pts->SetIsEnabled(true);
		if (GetSkillPoints(FNecoSkills::VE_StaminaRegen) >= MAX_SKILL) {
			STR_DEX_01_pts->SetIsEnabled(true);
		}
	}
}

void USkillsMenuWidget::OnAddSTR03() {
	STR03_pts->SetText(FText::FromString(FString::FromInt(
		OnAddSkillPoint(FNecoSkills::VE_Headshot)
	)));
}

void USkillsMenuWidget::OnAddDEX01() {
	int skillPoints = OnAddSkillPoint(FNecoSkills::VE_Stamina);
	DEX01_pts->SetText(FText::FromString(FString::FromInt(skillPoints)));
	if (skillPoints >= MAX_SKILL) DEX02_pts->SetIsEnabled(true);
}

void USkillsMenuWidget::OnAddDEX02() {
	int skillPoints = OnAddSkillPoint(FNecoSkills::VE_StaminaRegen);
	DEX02_pts->SetText(FText::FromString(FString::FromInt(skillPoints)));
	if (skillPoints >= MAX_SKILL) {
		DEX03_pts->SetIsEnabled(true);
		if (GetSkillPoints(FNecoSkills::VE_FireRate) >= MAX_SKILL) {
			STR_DEX_01_pts->SetIsEnabled(true);
		}
		if (GetSkillPoints(FNecoSkills::VE_HealHP) >= MAX_SKILL) {
			DEX_CON_01_pts->SetIsEnabled(true);
		}
	}
}

void USkillsMenuWidget::OnAddDEX03() {
	DEX03_pts->SetText(FText::FromString(FString::FromInt(
		OnAddSkillPoint(FNecoSkills::VE_MovementSpeed)
	)));
}

void USkillsMenuWidget::OnAddCON01() {
	int skillPoints = OnAddSkillPoint(FNecoSkills::VE_MaxHP);
	CON01_pts->SetText(FText::FromString(FString::FromInt(skillPoints)));
	if (skillPoints >= MAX_SKILL) CON02_pts->SetIsEnabled(true);
}

void USkillsMenuWidget::OnAddCON02() {
	int skillPoints = OnAddSkillPoint(FNecoSkills::VE_HealHP);
	CON02_pts->SetText(FText::FromString(FString::FromInt(skillPoints)));
	if (skillPoints >= MAX_SKILL) {
		CON03_pts->SetIsEnabled(true);
		if (GetSkillPoints(FNecoSkills::VE_StaminaRegen) >= MAX_SKILL) {
			DEX_CON_01_pts->SetIsEnabled(true);
		}
	}
}

void USkillsMenuWidget::OnAddCON03() {
	CON03_pts->SetText(FText::FromString(FString::FromInt(
		OnAddSkillPoint(FNecoSkills::VE_RegenHP)
	)));
}

void USkillsMenuWidget::OnAddSTR_DEX01() {
	STR_DEX_01_pts->SetText(FText::FromString(FString::FromInt(
		OnAddSkillPoint(FNecoSkills::VE_ReloadSpeed)
	)));
}

void USkillsMenuWidget::OnAddDEX_CON01() {
	DEX_CON_01_pts->SetText(FText::FromString(FString::FromInt(
		OnAddSkillPoint(FNecoSkills::VE_WeaponDamage)
	)));
}

void USkillsMenuWidget::OnInfoTextSTR01() {
	SkillNameText->SetText(FText::FromString("Paw Kit"));
	SkillDescriptionText->SetText(FText::FromString("Increase max reserve ammo by 10%"));
}

void USkillsMenuWidget::OnInfoTextSTR02() {
	SkillNameText->SetText(FText::FromString("Twitchy Paws"));
	SkillDescriptionText->SetText(FText::FromString("Increase fire rate by 2%"));
}

void USkillsMenuWidget::OnInfoTextSTR03() {
	SkillNameText->SetText(FText::FromString("BOOM"));
	SkillDescriptionText->SetText(FText::FromString("Increase headshot damage by 20%"));
}

void USkillsMenuWidget::OnInfoTextDEX01() {
	SkillNameText->SetText(FText::FromString("Marathon"));
	SkillDescriptionText->SetText(FText::FromString("Increase max stamina by 5%"));
}

void USkillsMenuWidget::OnInfoTextDEX02() {
	SkillNameText->SetText(FText::FromString("Second Wind"));
	SkillDescriptionText->SetText(FText::FromString("Increase Stamina regeneration by 5%"));
}

void USkillsMenuWidget::OnInfoTextDEX03() {
	SkillNameText->SetText(FText::FromString("Four Legged"));
	SkillDescriptionText->SetText(FText::FromString("Increase movement speed"));
}

void USkillsMenuWidget::OnInfoTextCON01() {
	SkillNameText->SetText(FText::FromString("Tis but a scratch"));
	SkillDescriptionText->SetText(FText::FromString("Increase max health by 15"));
}

void USkillsMenuWidget::OnInfoTextCON02() {
	SkillNameText->SetText(FText::FromString("PILK"));
	SkillDescriptionText->SetText(FText::FromString("Increase healing done by potions by 30"));
}

void USkillsMenuWidget::OnInfoTextCON03() {
	SkillNameText->SetText(FText::FromString("9th life"));
	SkillDescriptionText->SetText(FText::FromString("Add health regeneration"));
}

void USkillsMenuWidget::OnInfoTextSTR_DEX01() {
	SkillNameText->SetText(FText::FromString("Paw Maw"));
	SkillDescriptionText->SetText(FText::FromString("Increase reload speed by 2%"));
}

void USkillsMenuWidget::OnInfoTextDEX_CON01() {
	SkillNameText->SetText(FText::FromString("FMJ"));
	SkillDescriptionText->SetText(FText::FromString("Increase weapon damage by 20%"));
}

void USkillsMenuWidget::SetCharacter(AMainCharacter* aCharacter) {
	Character = aCharacter;
	SkillPointsPrompt->SetText(FText::FromString(FString::FromInt(
		Character->GetSkillUpgrades()->GetSkillPoints()
	)));
}
