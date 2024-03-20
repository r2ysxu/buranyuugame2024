// Fill out your copyright notice in the Description page of Project Settings.


#include "StartMainMenuWidget.h"
#include "../Actors/MainMenuActor.h"

#include "Components/Button.h"

void UStartMainMenuWidget::NativeConstruct() {
	PlaySingleButton->OnClicked.AddDynamic(this, &UStartMainMenuWidget::OnPlaySPClicked);
	PlayMultiplayerButton->OnClicked.AddDynamic(this, &UStartMainMenuWidget::OnPlayMPClicked);
}

void UStartMainMenuWidget::OnPlaySPClicked() {}

void UStartMainMenuWidget::OnPlayMPClicked() {
	SetVisibility(ESlateVisibility::Hidden);
	Parent->ChangeToMultiplayer();
}

void UStartMainMenuWidget::SetParent(AMainMenuActor* MenuActor) {
	Parent = MenuActor;
}
