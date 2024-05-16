// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "IntroPilotCharacter.generated.h"

UCLASS()
class NECOGOBLIN_API AIntroPilotCharacter : public ACharacter{
	GENERATED_BODY()

private:
	void SetupCharacterSkin(int SkinIndex);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TArray<class USkeletalMesh*> CharacterMeshes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TArray<class UAnimationAsset*> CharacterAnimAssets;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	AIntroPilotCharacter();
};
