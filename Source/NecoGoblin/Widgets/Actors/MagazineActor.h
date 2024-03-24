// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MagazineActor.generated.h"

UCLASS()
class NECOGOBLIN_API AMagazineActor : public APawn {
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneCaptureComponent2D* SceneCapturer;

	class AMainCharacter* Parent;

	virtual void BeginPlay() override;

public:	
	AMagazineActor();

	void SetParent(class AMainCharacter* Character);
	UFUNCTION(BlueprintCallable) float GetReloadUIFrame();
	UFUNCTION(BlueprintCallable) bool GetIsFiringWeapon();
	UFUNCTION(BlueprintCallable) bool GetIsReloading();
};
