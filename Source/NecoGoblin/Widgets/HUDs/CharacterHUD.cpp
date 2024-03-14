// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterHUD.h"
#include "../../Characters/Neco/MainCharacter.h"

#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"

void UCharacterHUD::NativeConstruct() {
	MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	MainCharacter->Delegate_HealthChange.AddDynamic(this, &UCharacterHUD::UpdateHealth);
	Healthbar->SetPercent(1.f);
}

void UCharacterHUD::UpdateHealth(float Health) {
	Healthbar->SetPercent(Health);
}

