// Fill out your copyright notice in the Description page of Project Settings.


#include "StartMultiplayerMenuWidget.h"
#include "../Actors/MainMenuActor.h"

#include "Components/Button.h"

void UStartMultiplayerMenuWidget::NativeConstruct() {
	BackButton->OnClicked.AddDynamic(this, &UStartMultiplayerMenuWidget::OnBackClicked);
}

void UStartMultiplayerMenuWidget::OnBackClicked() {
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("onBackClicked"));
	Parent->ClearMultiplayerMenu();
	Parent->ChangeToMenuCamera();
}

void UStartMultiplayerMenuWidget::OnMPHostClicked() {}

void UStartMultiplayerMenuWidget::OnMPSearchClicked() {}

void UStartMultiplayerMenuWidget::SetParent(AMainMenuActor* MenuActor) {
	Parent = MenuActor;
}
