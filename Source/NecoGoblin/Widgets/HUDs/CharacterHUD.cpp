// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterHUD.h"
#include "../../Characters/Neco/MainCharacter.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"

void UCharacterHUD::NativeConstruct() {
	MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());

	MainCharacter->Delegate_HealthChange.AddDynamic(this, &UCharacterHUD::UpdateHealth);
	MainCharacter->GetWeapon()->Delegate_ReserveAmmoChange.AddDynamic(this, &UCharacterHUD::UpdateReserveAmmo);

	Healthbar->SetPercent(MainCharacter->GetHealthPercentage());
	ReserveAmmoCount->SetText(FText::FromString(FString::FromInt(MainCharacter->GetWeapon()->GetReserveAmmo())));
}

void UCharacterHUD::UpdateHealth(float Health) {
	Healthbar->SetPercent(Health);
}

void UCharacterHUD::UpdateReserveAmmo(int ReserveAmmo) {
	ReserveAmmoCount->SetText(FText::FromString(FString::FromInt(ReserveAmmo)));
}

void UCharacterHUD::SwitchToCharacterIndex(int Index) {
	MagazineSwitcher->SetActiveWidgetIndex(Index);
}

