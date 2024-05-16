// Fill out your copyright notice in the Description page of Project Settings.


#include "IntroPilotCharacter.h"
#include "MainCharacter.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AIntroPilotCharacter::AIntroPilotCharacter() {
	PrimaryActorTick.bCanEverTick = false;

}

void AIntroPilotCharacter::SetupCharacterSkin(int SkinIndex) {
	GetMesh()->SetSkeletalMesh(CharacterMeshes[SkinIndex]);
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	GetMesh()->SetAnimation(CharacterAnimAssets[SkinIndex]);
	GetMesh()->Play(true);
}

// Called when the game starts or when spawned
void AIntroPilotCharacter::BeginPlay() {
	Super::BeginPlay();
	AMainCharacter* mc = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (IsValid(mc)) {
		SetupCharacterSkin(mc->GetCharacterSkinIndex());
	}
}