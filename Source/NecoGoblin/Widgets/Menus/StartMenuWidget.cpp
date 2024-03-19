// Fill out your copyright notice in the Description page of Project Settings.


#include "StartMenuWidget.h"
#include "../Actors/MainMenuActor.h"

#include "Components/Button.h"

void UStartMenuWidget::NativeConstruct() {
	StartButton->OnClicked.AddDynamic(this, &UStartMenuWidget::OnStartClicked);
}

void UStartMenuWidget::OnStartClicked() {
	Parent->ChangeToMenuCamera();
	SetVisibility(ESlateVisibility::Hidden);
	RemoveFromParent();
}

void UStartMenuWidget::SetParent(AMainMenuActor* MenuActor) {
	Parent = MenuActor;
}
