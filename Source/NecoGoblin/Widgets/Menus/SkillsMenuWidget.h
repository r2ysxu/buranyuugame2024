// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Characters/Neco/UpgradeSkillComponent.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillsMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class NECOGOBLIN_API USkillsMenuWidget : public UUserWidget {
	GENERATED_BODY()

private:
	const int MAX_SKILL = 5;

	int OnAddSkillPoint(enum FNecoSkills Skill);
	int GetSkillPoints(enum FNecoSkills Skill);
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Skill_STR_01;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Skill_STR_02;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Skill_STR_03;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Skill_DEX_01;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Skill_DEX_02;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Skill_DEX_03;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Skill_CON_01;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Skill_CON_02;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Skill_CON_03;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Skill_STR_DEX_01;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Skill_DEX_CON_01;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* STR01_pts;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* STR02_pts;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* STR03_pts;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* DEX01_pts;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* DEX02_pts;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* DEX03_pts;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* CON01_pts;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* CON02_pts;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* CON03_pts;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* STR_DEX_01_pts;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* DEX_CON_01_pts;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* SkillNameText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* SkillDescriptionText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* SkillPointsPrompt;

	class AMainCharacter* Character;

	virtual void NativeConstruct() override;

	UFUNCTION() void OnAddSTR01();
	UFUNCTION() void OnAddSTR02();
	UFUNCTION() void OnAddSTR03();
	UFUNCTION() void OnAddDEX01();
	UFUNCTION() void OnAddDEX02();
	UFUNCTION() void OnAddDEX03();
	UFUNCTION() void OnAddCON01();
	UFUNCTION() void OnAddCON02();
	UFUNCTION() void OnAddCON03();
	UFUNCTION() void OnAddSTR_DEX01();
	UFUNCTION() void OnAddDEX_CON01();

	UFUNCTION() void OnInfoTextSTR01();
	UFUNCTION() void OnInfoTextSTR02();
	UFUNCTION() void OnInfoTextSTR03();
	UFUNCTION() void OnInfoTextDEX01();
	UFUNCTION() void OnInfoTextDEX02();
	UFUNCTION() void OnInfoTextDEX03();
	UFUNCTION() void OnInfoTextCON01();
	UFUNCTION() void OnInfoTextCON02();
	UFUNCTION() void OnInfoTextCON03();
	UFUNCTION() void OnInfoTextSTR_DEX01();
	UFUNCTION() void OnInfoTextDEX_CON01();

public:

	void SetCharacter(class AMainCharacter* aCharacter);
};
