// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuActor.h"
#include "../Menus/StartMainMenuWidget.h"
#include "../Menus/StartMultiplayerMenuWidget.h"
#include "../Menus/StartMenuWidget.h"

#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

// Sets default values
AMainMenuActor::AMainMenuActor() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	LogoMenuWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("LogoMenuWidget"));
	StartMenuWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("StartMenuWidget"));
	GameMenuWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("GameMenuWidget"));
	MPMenuWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("MPMenuWidget"));

	LogoMenuWidget->SetupAttachment(GetRootComponent());
	StartMenuWidget->SetupAttachment(LogoMenuWidget);
	GameMenuWidget->SetupAttachment(LogoMenuWidget);
	MPMenuWidget->SetupAttachment(LogoMenuWidget);
}

// Called when the game starts or when spawned
void AMainMenuActor::BeginPlay() {
	Super::BeginPlay();
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	controller->SetViewTarget(StartMenuCamera);
	Cast<UStartMenuWidget>(StartMenuWidget->GetWidget())->SetParent(this);
	Cast<UStartMultiplayerMenuWidget>(MPMenuWidget->GetWidget())->SetParent(this);
	Cast<UStartMainMenuWidget>(GameMenuWidget->GetWidget())->SetParent(this);
}

void AMainMenuActor::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	if (LoadingWidget) LoadingWidget->RemoveFromParent();
}

void AMainMenuActor::ChangeToMenuCamera() {
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	controller->SetViewTargetWithBlend(MainMenuCamera, 1.f);
	FInputModeGameAndUI mode = FInputModeGameAndUI();
	mode.SetWidgetToFocus(GameMenuWidget->GetSlateWidget());
	controller->SetInputMode(mode);
	GameMenuWidget->GetWidget()->SetVisibility(ESlateVisibility::Visible);
}

void AMainMenuActor::ChangeToMultiplayer() {
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTargetWithBlend(MPMenuCamera, 1.f);
}

void AMainMenuActor::ChangeToSingleplayer() {
	LoadingWidget = CreateWidget<UUserWidget>(GetWorld(), LoadingWidgetClass);
	if (LoadingWidget) { 
		LoadingWidget->AddToViewport();
	}
	UGameplayStatics::OpenLevel(GetWorld(), "IslandMap");
}
