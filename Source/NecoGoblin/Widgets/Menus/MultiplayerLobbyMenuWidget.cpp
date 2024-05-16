// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerLobbyMenuWidget.h"
#include "../../Gamemodes/MultiplayerGameMode.h"
#include "../../GameInstance/NGGameInstance.h"
#include "../../Characters/Ally/MainCharacter.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMultiplayerLobbyMenuWidget::NativeConstruct() {
	StartButton->OnClicked.AddDynamic(this, &UMultiplayerLobbyMenuWidget::OnStartGameClicked);
	BackButton->OnClicked.AddDynamic(this, &UMultiplayerLobbyMenuWidget::OnBackClicked);

	NextCharacterButton->OnClicked.AddDynamic(this, &UMultiplayerLobbyMenuWidget::OnNextClicked);
	PrevCharacterButton->OnClicked.AddDynamic(this, &UMultiplayerLobbyMenuWidget::OnPrevClicked);
}

void UMultiplayerLobbyMenuWidget::OnStartGameClicked() {
	StartButton->SetIsEnabled(false);
	if (AMultiplayerGameMode* gamemode = Cast<AMultiplayerGameMode>(UGameplayStatics::GetGameMode(GetWorld()))) {
		gamemode->LoadIntoMPLevel("IslandMap_MP");
	}
	RemoveFromParent();
	//Cast<UNGGameInstance>(GetGameInstance())->JumpToLevel("/Game/NecoGoblin/Maps/MultiplayerMaps/IslandMap_MP?listen");
}

void UMultiplayerLobbyMenuWidget::SetParent(AMainCharacter* Character) {
	MainCharacter = Character;
	StartButton->SetIsEnabled(MainCharacter->HasAuthority());
}

void UMultiplayerLobbyMenuWidget::OnNextClicked() {
	if (MainCharacter) {
		SelectedCharacterSkinIndex = ++SelectedCharacterSkinIndex % MainCharacter->GetCharacterSkinSize();
		MainCharacter->ChangeCharacterSkin(SelectedCharacterSkinIndex);
	}
}

void UMultiplayerLobbyMenuWidget::OnPrevClicked() {
	if (MainCharacter) {
		SelectedCharacterSkinIndex = --SelectedCharacterSkinIndex;
		if (SelectedCharacterSkinIndex < 0) SelectedCharacterSkinIndex = MainCharacter->GetCharacterSkinSize() - 1;
		MainCharacter->ChangeCharacterSkin(SelectedCharacterSkinIndex);
	}
}

void UMultiplayerLobbyMenuWidget::OnBackClicked() {
	if (MainCharacter->HasAuthority()) {
		Cast<UNGGameInstance>(GetGameInstance())->QuitSession();
		UGameplayStatics::OpenLevel(GetWorld(), "MenuMapWorld");
	}
}
