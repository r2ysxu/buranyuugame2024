// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponCrate.h"
#include "../../Widgets/HUDs/WeaponHUD.h"
#include "../../Weapons/Ranged/FirearmWeapon.h"
#include "../../Characters/Ally/MainCharacter.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AWeaponCrate::BeginPlay() {
	Super::BeginPlay();
	WeaponData = WeaponDataTable->FindRow<FFirearmWeaponData>(WeaponKey, FString("MainFirearm"), true);

	InfoHudWidget = CreateWidget<UWeaponHUD>(GetWorld(), InfoHudWidgetClass);
	if (IsValid(InfoHudWidget)) {
		InfoHudWidget->AddToViewport();
		InfoHudWidget->SetVisibility(ESlateVisibility::Hidden);
		InfoHudWidget->SetCurrentWeaponStats(GetStats());
	}
}

void AWeaponCrate::OnWithinRange(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	AMainCharacter* mainCharacter = Cast<AMainCharacter>(actor);
	if (IsValid(mainCharacter)) {
		mainCharacter->SetChangableWeapon(WeaponKey);
		InfoHudWidget->SetVisibility(ESlateVisibility::Visible);
		InfoHudWidget->SetPlayerWeaponStats(mainCharacter->GetFirearmStats());
	}
}

void AWeaponCrate::OnOutsideRange(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) {
	AMainCharacter* mainCharacter = Cast<AMainCharacter>(actor);
	if (IsValid(mainCharacter)) {
		mainCharacter->SetChangableWeapon(FName());
		InfoHudWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

FFirearmStats AWeaponCrate::GetStats() {
	FFirearmStats stats = FFirearmStats();
	if (WeaponData) {
		stats.Name = WeaponData->WeaponName;
		stats.DamageP = WeaponData->BaseDamage / stats.DamageP;
		stats.FireRateP = 1.f - WeaponData->FireRate * stats.FireRateP;
		stats.HandlingP = stats.HandlingP - (WeaponData->RecoilYawVariance + WeaponData->RecoiPitchVariance);
		stats.InfoText = WeaponData->StatInfo;
	}
	return stats;
}
