// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoCrate.h"
#include "../../Characters/Neco/MainCharacter.h"

#include "Blueprint/UserWidget.h"

// Sets default values
AAmmoCrate::AAmmoCrate() {

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("CrateMesh");
	MeshComponent->SetupAttachment(GetRootComponent());
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AAmmoCrate::BeginPlay() {
	Super::BeginPlay();
	if (Mesh) MeshComponent->SetStaticMesh(Mesh);
	InteractHudWidget = CreateWidget<UUserWidget>(GetWorld(), InteractHudWidgetClass);
	if (InteractHudWidget) {
		InteractHudWidget->AddToViewport();
		InteractHudWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AAmmoCrate::OnWithinRange(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	AMainCharacter* mainCharacter = Cast<AMainCharacter>(actor);
	if (IsValid(mainCharacter)) {
		mainCharacter->CanRefillAmmo(true);
		InteractHudWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AAmmoCrate::OnOutsideRange(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) {
	AMainCharacter* mainCharacter = Cast<AMainCharacter>(actor);
	if (IsValid(mainCharacter)) {
		mainCharacter->CanRefillAmmo(false);
		InteractHudWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

