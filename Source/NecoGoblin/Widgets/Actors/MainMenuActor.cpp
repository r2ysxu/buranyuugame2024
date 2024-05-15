// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuActor.h"
#include "../Menus/StartMainMenuWidget.h"
#include "../Menus/StartMultiplayerMenuWidget.h"

#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

// Sets default values
AMainMenuActor::AMainMenuActor() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AMainMenuActor::BeginPlay() {
	Super::BeginPlay();
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	controller->SetViewTarget(StartMenuCamera);
}

void AMainMenuActor::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	if (LoadingWidget) LoadingWidget->RemoveFromParent();
}

void AMainMenuActor::ChangeToMenuCamera() {
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTargetWithBlend(MainMenuCamera, 1.f);
	StartMainMenuWidget = CreateWidget<UStartMainMenuWidget>(GetWorld(), StartMainMenuWidgetClass);
	if (StartMainMenuWidget) {
		StartMainMenuWidget->AddToViewport();
		StartMainMenuWidget->SetParent(this);
	}
}

void AMainMenuActor::ChangeToMultiplayer() {
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTargetWithBlend(MPMenuCamera, 1.f);
	if (StartMainMenuWidget) {
		StartMainMenuWidget->RemoveFromParent();
	}
}

void AMainMenuActor::ChangeToSingleplayer() {
	LoadingWidget = CreateWidget<UUserWidget>(GetWorld(), LoadingWidgetClass);
	if (LoadingWidget) { 
		LoadingWidget->AddToViewport();
	}
	UGameplayStatics::OpenLevel(GetWorld(), "IslandMap");
}
