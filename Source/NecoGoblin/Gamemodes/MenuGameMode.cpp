// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuGameMode.h"

#include "Kismet/GameplayStatics.h"

void AMenuGameMode::StartPlay() {
	Super::StartPlay();
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	controller->bShowMouseCursor = true;
}

AMenuGameMode::AMenuGameMode() {
	DefaultPawnClass = nullptr;
}
