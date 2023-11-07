// Fill out your copyright notice in the Description page of Project Settings.

#include "../../Characters/Neco/MainCharacter.h"
#include "../../NecoGoblinGameMode.h"

#include "MainMenuWidget.h"
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
	FInputModeUIOnly* inputMode = new FInputModeUIOnly();
	inputMode->SetWidgetToFocus(SkipIntroWidget->GetCachedWidget());
	controller->SetInputMode(*inputMode);
	controller->bShowMouseCursor = false;
}
