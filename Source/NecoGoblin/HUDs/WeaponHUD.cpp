// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponHUD.h"
#include "../Weapons/Ranged/FirearmWeapon.h"

void UWeaponHUD::SetCurrentWeaponStats(FFirearmStats Stats) {
	CurrentWeaponStats = Stats;
}

void UWeaponHUD::SetPlayerWeaponStats(FFirearmStats Stats) {
	PlayerWeaponStats = Stats;
}

FFirearmStats UWeaponHUD::GetCurrentWeaponStats() {
	return CurrentWeaponStats;
}

FFirearmStats UWeaponHUD::GetPlayerWeaponStats() {
	return PlayerWeaponStats;
}