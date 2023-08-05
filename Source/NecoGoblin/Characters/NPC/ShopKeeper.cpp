// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopKeeper.h"
#include "../Neco/MainCharacter.h"
#include "Components/SphereComponent.h"
#include "Blueprint/UserWidget.h"

// Sets default values
AShopKeeper::AShopKeeper() {
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ShopRangeBox = CreateDefaultSubobject<USphereComponent>(TEXT("ShopRangeBox"));
	ShopRangeBox->SetSphereRadius(80.f);
	ShopRangeBox->AddLocalOffset(FVector(120.f, 0.0, 0.f));
	ShopRangeBox->SetupAttachment(GetRootComponent());

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::ProbeOnly);

}

// Called when the game starts or when spawned
void AShopKeeper::BeginPlay() {
	Super::BeginPlay();
	
	ShopRangeBox->OnComponentBeginOverlap.AddDynamic(this, &AShopKeeper::OnWithinShopRange);
	ShopRangeBox->OnComponentEndOverlap.AddDynamic(this, &AShopKeeper::OnOutsideShopRange);

	ShopOpenHudWidget = CreateWidget<UUserWidget>(GetWorld(), ShopOpenHudWidgetClass);
	if (ShopOpenHudWidget) {
		ShopOpenHudWidget->AddToViewport();
		ShopOpenHudWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AShopKeeper::OnWithinShopRange(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	AMainCharacter* mainCharacter = Cast<AMainCharacter>(actor);
	if (IsValid(mainCharacter)) {
		mainCharacter->GetShopUpgrades()->SetCanShop(true);
		ShopOpenHudWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AShopKeeper::OnOutsideShopRange(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) {
	AMainCharacter* mainCharacter = Cast<AMainCharacter>(actor);
	if (IsValid(mainCharacter)) {
		mainCharacter->GetShopUpgrades()->SetCanShop(false);
		ShopOpenHudWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

