// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoCrate.h"
#include "../Neco/MainCharacter.h"
#include "Components/SphereComponent.h"
#include "Blueprint/UserWidget.h"

// Sets default values
AAmmoCrate::AAmmoCrate() {
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("OneHandWeaponRoot"));

	InteractRangeBox = CreateDefaultSubobject<USphereComponent>(TEXT("ShopRangeBox"));
	InteractRangeBox->SetSphereRadius(80.f);
	InteractRangeBox->SetupAttachment(GetRootComponent());

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("CrateMesh");
	MeshComponent->SetupAttachment(GetRootComponent());
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AAmmoCrate::BeginPlay() {
	Super::BeginPlay();

	if (Mesh) MeshComponent->SetStaticMesh(Mesh);
	
	InteractRangeBox->OnComponentBeginOverlap.AddDynamic(this, &AAmmoCrate::OnWithinRange);
	InteractRangeBox->OnComponentEndOverlap.AddDynamic(this, &AAmmoCrate::OnOutsideRange);

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

