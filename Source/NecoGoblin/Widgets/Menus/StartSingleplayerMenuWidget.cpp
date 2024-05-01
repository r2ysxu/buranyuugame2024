// Fill out your copyright notice in the Description page of Project Settings.

#include "StartSingleplayerMenuWidget.h"
#include "../../Characters/Ally/MainCharacter.h"
#include "../../Gamemodes/SingleplayerGameMode.h"

#include "Kismet/GameplayStatics.h"


void UStartSingleplayerMenuWidget::NativeConstruct() {
	SkipIntroWidget = CreateWidget<UUserWidget>(GetWorld(), SkipIntroWidgetClass);
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
