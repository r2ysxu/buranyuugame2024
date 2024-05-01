// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSwitcherMenuWidget.h"
#include "../../Characters/Ally/MainCharacter.h"
#include "Components/Button.h"

void UCharacterSwitcherMenuWidget::NativeConstruct() {
	NextCharacterButton->OnClicked.AddDynamic(this, &UCharacterSwitcherMenuWidget::OnNextClicked);
	PrevCharacterButton->OnClicked.AddDynamic(this, &UCharacterSwitcherMenuWidget::OnPrevClicked);
}

void UCharacterSwitcherMenuWidget::SetParent(AMainCharacter* Character) {
	MainCharacter = Character;
}

void UCharacterSwitcherMenuWidget::OnNextClicked() {
	if (MainCharacter) {
		SelectedCharacterSkinIndex = ++SelectedCharacterSkinIndex % MainCharacter->GetCharacterSkinSize();
		MainCharacter->ChangeCharacterSkin(SelectedCharacterSkinIndex);
	}
}

void UCharacterSwitcherMenuWidget::OnPrevClicked() {
	if (MainCharacter) {
		SelectedCharacterSkinIndex = --SelectedCharacterSkinIndex;
		if (SelectedCharacterSkinIndex < 0) SelectedCharacterSkinIndex = MainCharacter->GetCharacterSkinSize() - 1;
		MainCharacter->ChangeCharacterSkin(SelectedCharacterSkinIndex);
	}
}
