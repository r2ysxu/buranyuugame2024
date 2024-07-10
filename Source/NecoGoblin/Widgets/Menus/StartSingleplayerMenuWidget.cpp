// Fill out your copyright notice in the Description page of Project Settings.

#include "StartSingleplayerMenuWidget.h"
#include "../../Characters/Ally/MainCharacter.h"
#include "../../Gamemodes/SingleplayerGameMode.h"
#include "../../GameInstance/NGGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"


void UStartSingleplayerMenuWidget::NativeConstruct() {
	SkipIntroWidget = CreateWidget<UUserWidget>(GetWorld(), SkipIntroWidgetClass);
	OptionsEndless->OnClicked.AddDynamic(this, &UStartSingleplayerMenuWidget::OnModeChanged);
	NextCharButton->OnClicked.AddDynamic(this, &UStartSingleplayerMenuWidget::NextCharacter);
	PrevCharButton->OnClicked.AddDynamic(this, &UStartSingleplayerMenuWidget::PrevCharacter);
	SetSelectedCharacterText(0);
}

void UStartSingleplayerMenuWidget::OnStartGame() {
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	APlayerController* controller = GetOwningPlayer();
	controller->DisableInput(controller);
	RemoveFromParent();
	SkipIntroWidget->AddToViewport();
	controller->bShowMouseCursor = false;
}

void UStartSingleplayerMenuWidget::OnModeChanged() {
	IsEndlessMode = !IsEndlessMode;
	Cast<ASingleplayerGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->SetEndlessMode(IsEndlessMode);
	OptionEndlessModeText->SetText(FText::FromString(IsEndlessMode ? TEXT("Endless") : TEXT("Standard")));
}

float UStartSingleplayerMenuWidget::GetMusicVolume() {
	return Cast<UNGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetMusicVolume();
}

void UStartSingleplayerMenuWidget::NextCharacter() {
	AMainCharacter* mc = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (mc) {
		mc->ChangeCharacterSkin(1);
		SetSelectedCharacterText(mc->GetCharacterSkinIndex());
	}
}

void UStartSingleplayerMenuWidget::PrevCharacter() {
	AMainCharacter* mc = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (mc) {
		mc->ChangeCharacterSkin(-1);
		SetSelectedCharacterText(mc->GetCharacterSkinIndex());
	}
}

void UStartSingleplayerMenuWidget::SetSelectedCharacterText(int SkinIndex) {
	switch (SkinIndex) {
	case 0: 
		CharacterText->SetText(FText::FromString(TEXT("Indentured Rabbit")));
		CharacterJobText->SetText(FText::FromString(TEXT("Janitor, Pilot")));
		break;
	case 1:
		CharacterText->SetText(FText::FromString(TEXT("Nutty Squirrel")));
		CharacterJobText->SetText(FText::FromString(TEXT("Explosive Expert")));
		break;
	case 2:
		CharacterText->SetText(FText::FromString(TEXT("Stowaway Raccoon")));
		CharacterJobText->SetText(FText::FromString(TEXT("Who dis?")));
		break;
	case 3:
		CharacterText->SetText(FText::FromString(TEXT("Neko Aku")));
		CharacterJobText->SetText(FText::FromString(TEXT("Burenyuu, Boss")));
		break;
	}
	CharacterProfilePics->SetActiveWidgetIndex(SkinIndex);
}