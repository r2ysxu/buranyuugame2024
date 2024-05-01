// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuWidget.h"
#include "../../Characters/Ally/MainCharacter.h"
#include "../../NecoGoblinGameMode.h"

#include "Kismet/GameplayStatics.h"


void UMainMenuWidget::NativeConstruct() {
	SkipIntroWidget = CreateWidget<UUserWidget>(GetWorld(), SkipIntroWidgetClass);
}

void UMainMenuWidget::OnStartGame() {
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	APlayerController* controller = GetOwningPlayer();
	controller->DisableInput(controller);
	RemoveFromParent();
	SkipIntroWidget->AddToViewport();
	controller->bShowMouseCursor = false;
}

void UMainMenuWidget::SetGameMode(bool IsEndlessMode) {
	Cast<ANecoGoblinGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->SetEndlessMode(IsEndlessMode);
}
