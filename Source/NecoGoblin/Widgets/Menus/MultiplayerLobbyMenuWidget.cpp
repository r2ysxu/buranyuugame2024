// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerLobbyMenuWidget.h"
#include "../../Gamemodes/MultiplayerGameMode.h"
#include "../../GameInstance/NGGameInstance.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMultiplayerLobbyMenuWidget::NativeConstruct() {
	StartButton->OnClicked.AddDynamic(this, &UMultiplayerLobbyMenuWidget::OnStartGameClicked);
}

void UMultiplayerLobbyMenuWidget::OnStartGameClicked() {
	StartButton->SetIsEnabled(false);
	if (AMultiplayerGameMode* gamemode = Cast<AMultiplayerGameMode>(UGameplayStatics::GetGameMode(GetWorld()))) {
		gamemode->LoadIntoMPLevel("IslandMap_MP");
	}
	RemoveFromParent();
	//Cast<UNGGameInstance>(GetGameInstance())->JumpToLevel("/Game/NecoGoblin/Maps/MultiplayerMaps/IslandMap_MP?listen");
}
