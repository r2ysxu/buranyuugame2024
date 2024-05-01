// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class NECOGOBLIN_API UCharacterAnimInstance : public UAnimInstance {
	GENERATED_BODY()

protected:
	class AMainCharacter* Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAiming;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Pitch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Direction = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsReloading;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsFalling;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsFlinching;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAlive;

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
};
