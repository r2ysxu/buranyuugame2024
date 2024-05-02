// Fill out your copyright notice in the Description page of Project Settings.

#include "StartSingleplayerMenuWidget.h"
#include "../../Characters/Ally/MainCharacter.h"
#include "../../Gamemodes/SingleplayerGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Components/CheckBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"


void UStartSingleplayerMenuWidget::NativeConstruct() {
	SkipIntroWidget = CreateWidget<UUserWidget>(GetWorld(), SkipIntroWidgetClass);
	Options_Endless->OnCheckStateChanged.AddDynamic(this, &UStartSingleplayerMenuWidget::SetGameMode);
	NextCharButton->OnClicked.AddDynamic(this, &UStartSingleplayerMenuWidget::NextCharacter);
	PrevCharButton->OnClicked.AddDynamic(this, &UStartSingleplayerMenuWidget::PrevCharacter);
	SetSelectedCharacterText();
}

void UStartSingleplayerMenuWidget::OnStartGame() {
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	APlayerController* controller = GetOwningPlayer();
	controller->DisableInput(controller);
	RemoveFromParent();
	SkipIntroWidget->AddToViewport();
	controller->bShowMouseCursor = false;
}

void UStartSingleplayerMenuWidget::SetGameMode(bool IsEndlessMode) {
	Cast<ASingleplayerGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->SetEndlessMode(IsEndlessMode);
}

void UStartSingleplayerMenuWidget::NextCharacter() {
	AMainCharacter* mc = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (mc) {
		SelectedCharacterSkinIndex = ++SelectedCharacterSkinIndex % mc->GetCharacterSkinSize();
		mc->ChangeCharacterSkin(SelectedCharacterSkinIndex);
		SetSelectedCharacterText();
	}
}

void UStartSingleplayerMenuWidget::PrevCharacter() {
	AMainCharacter* mc = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (mc) {
		SelectedCharacterSkinIndex = --SelectedCharacterSkinIndex % mc->GetCharacterSkinSize();
		if (SelectedCharacterSkinIndex < 0) SelectedCharacterSkinIndex = mc->GetCharacterSkinSize() - 1;
		mc->ChangeCharacterSkin(SelectedCharacterSkinIndex);
		SetSelectedCharacterText();
	}
}

void UStartSingleplayerMenuWidget::SetSelectedCharacterText() {
	switch (SelectedCharacterSkinIndex) {
	case 0: CharacterText->SetText(FText::FromString(TEXT("Indentured Rabbit"))); break;
	case 1: CharacterText->SetText(FText::FromString(TEXT("Nutty Squirrel"))); break;
	case 2: CharacterText->SetText(FText::FromString(TEXT("Stowaway Raccoon"))); break;
	case 3: CharacterText->SetText(FText::FromString(TEXT("Neko Aku"))); break;
	}
}