// Fill out your copyright notice in the Description page of Project Settings.


#include "MagazineActor.h"
#include "../../Characters/Neco/MainCharacter.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/TextureRenderTarget2D.h"

// Sets default values
AMagazineActor::AMagazineActor() {
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* meshScene = CreateDefaultSubobject<USceneComponent>(TEXT("MeshScene"));
	SetRootComponent(meshScene);

	SceneCapturer = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapturer"));
	SceneCapturer->SetupAttachment(GetRootComponent());

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());

	SceneCapturer->ShowOnlyActorComponents(this);
}

void AMagazineActor::SetParent(AMainCharacter* Character) {
	Parent = Character;
	SceneCapturer->TextureTarget = LoadObject<UTextureRenderTarget2D>(GetWorld(), *FString::Printf(TEXT("/Game/NecoGoblin/HUD/MagazineUI/RT_MagazineUI%d"), Parent->GetCharacterIndex()));
}

float AMagazineActor::GetReloadUIFrame() {
	if (IsValid(Parent)) return Parent->GetReloadUIFrame();
	return 0;
}

bool AMagazineActor::GetIsFiringWeapon() {
	if (IsValid(Parent)) return Parent->GetIsFiringWeapon();
	return false;
}

bool AMagazineActor::GetIsReloading() {
	if (IsValid(Parent)) return Parent->GetIsReloading();
	return false;
}

// Called when the game starts or when spawned
void AMagazineActor::BeginPlay() {
	Super::BeginPlay();	
}

