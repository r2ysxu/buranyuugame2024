// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"
#include "MainCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

void UCharacterAnimInstance::NativeInitializeAnimation() {
	Character = Cast<AMainCharacter>(TryGetPawnOwner());
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
	if (Character) {
		Speed = Character->GetVelocity().Length();
		Pitch = Character->GetPlayerPitch();
		IsAiming = Character->GetIsAimMode();
		IsReloading = Character->GetIsReloading();
		IsFlinching = Character->GetIsFlinching();
		IsAlive = Character->GetIsAlive();
		IsFalling = Character->GetMovementComponent()->IsFalling();
	}
}
