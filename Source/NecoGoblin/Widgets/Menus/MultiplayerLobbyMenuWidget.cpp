// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerLobbyMenuWidget.h"
#include "../../GameInstance/NGGameInstance.h"
#include "../../Characters/Ally/MainCharacter.h"
#include "../../Controllers/MainPlayerController.h"

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
	SetVisibility(ESlateVisibility::Hidden);
	Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))
		->GameStart_Delegate.Broadcast();
}

void UMultiplayerLobbyMenuWidget::CheckCanStart(bool CanStart) {
	StartButton->SetIsEnabled(CanStart);
}

void UMultiplayerLobbyMenuWidget::OnNextClicked() {
	Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
		->ChangeCharacterSkin(1);
}

void UMultiplayerLobbyMenuWidget::OnPrevClicked() {
	Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
		->ChangeCharacterSkin(-1);
}

void UMultiplayerLobbyMenuWidget::OnBackClicked() {
	Cast<UNGGameInstance>(GetGameInstance())->QuitSession();
	UGameplayStatics::OpenLevel(GetWorld(), "MenuMapWorld");
}
