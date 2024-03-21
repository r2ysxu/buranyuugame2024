// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerLobbyMenuWidget.h"
#include "../../GameInstance/NGGameInstance.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMultiplayerLobbyMenuWidget::NativeConstruct() {
	StartButton->OnClicked.AddDynamic(this, &UMultiplayerLobbyMenuWidget::OnStartGameClicked);
}

void UMultiplayerLobbyMenuWidget::OnStartGameClicked() {
	Cast<UNGGameInstance>(GetGameInstance())->JumpToLevel("/Game/NecoGoblin/Maps/IslandMap?listen");
}
