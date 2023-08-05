// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeShopComponent.h"
#include "MainCharacter.h"
#include "Blueprint/UserWidget.h"

// Sets default values for this component's properties
UUpgradeShopComponent::UUpgradeShopComponent() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UUpgradeShopComponent::SetParentCharacter(AMainCharacter* character) {
	ParentCharacter = character;
}


// Called when the game starts
void UUpgradeShopComponent::BeginPlay() {
	Super::BeginPlay();
}

void UUpgradeShopComponent::SetCanShop(bool canShop) {
	CanShop = canShop;
}

void UUpgradeShopComponent::EnterShopScreen(UUserWidget* shopHudWidget) {
	ShopHudWidget = shopHudWidget;
	APlayerController* controller = Cast<APlayerController>(ParentCharacter->GetController());
	controller->bShowMouseCursor = true;
	ShopHudWidget->SetVisibility(ESlateVisibility::Visible);
	controller->SetInputMode(FInputModeUIOnly());
}

void UUpgradeShopComponent::ExitShopScreen() {
	APlayerController* controller = Cast<APlayerController>(ParentCharacter->GetController());
	controller->bShowMouseCursor = false;
	ShopHudWidget->SetVisibility(ESlateVisibility::Hidden);
	controller->SetInputMode(FInputModeGameOnly());
	ShopHudWidget = nullptr;
}

bool UUpgradeShopComponent::IncrementPoints(int points) {
	Points += points;
	return true;
}

bool UUpgradeShopComponent::DecrementPoints(int points) {
	if (Points - points < 0) return false;
	Points -= points;
	return true;
}


void UUpgradeShopComponent::HealToFull(int points) {
	if (DecrementPoints(points)) ParentCharacter->HealToFull();
}

void UUpgradeShopComponent::UpgradeWeaponDamage(int points, float additionalDamage) {
	if (DecrementPoints(points)) ParentCharacter->UpgradeWeaponDamage(additionalDamage);
}

void UUpgradeShopComponent::UpgradeHealth(int points, int additionalHealth) {
	if (DecrementPoints(points)) ParentCharacter->IncreaseMaxHealth(additionalHealth);
}