// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponHUD.generated.h"

/**
 * 
 */
UCLASS()
class NECOGOBLIN_API UWeaponHUD : public UUserWidget {
	GENERATED_BODY()
	
protected:
	struct FFirearmStats CurrentWeaponStats;
	struct FFirearmStats PlayerWeaponStats;

public:
	void SetCurrentWeaponStats(struct FFirearmStats Stats);
	void SetPlayerWeaponStats(struct FFirearmStats Stats);
	UFUNCTION(BlueprintCallable) struct FFirearmStats GetCurrentWeaponStats();
	UFUNCTION(BlueprintCallable) struct FFirearmStats GetPlayerWeaponStats();
};
