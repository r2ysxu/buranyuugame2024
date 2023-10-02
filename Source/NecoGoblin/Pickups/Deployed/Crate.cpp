// Fill out your copyright notice in the Description page of Project Settings.


#include "Crate.h"
#include "Components/SphereComponent.h"
#include "Blueprint/UserWidget.h"

// Sets default values
ACrate::ACrate() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("OneHandWeaponRoot"));

	InteractRangeBox = CreateDefaultSubobject<USphereComponent>(TEXT("CrateRangeBox"));
	InteractRangeBox->SetSphereRadius(80.f);
	InteractRangeBox->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void ACrate::BeginPlay() {
	Super::BeginPlay();
	InteractRangeBox->OnComponentBeginOverlap.AddDynamic(this, &ACrate::OnWithinRange);
	InteractRangeBox->OnComponentEndOverlap.AddDynamic(this, &ACrate::OnOutsideRange);
}

void ACrate::OnWithinRange(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {}

void ACrate::OnOutsideRange(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) {}