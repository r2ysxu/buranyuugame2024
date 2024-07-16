// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Crate.h"
#include "WeaponCrate.generated.h"

/**
 * 
 */
UCLASS()
class NECOGOBLIN_API AWeaponCrate : public ACrate {
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FName WeaponKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class UDataTable* WeaponDataTable = nullptr;
	struct FFirearmWeaponData* WeaponData;

	virtual void BeginPlay() override;

public:

	struct FFirearmStats GetStats();
	void OnWithinRange(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	void OnOutsideRange(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) override;
};