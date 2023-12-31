// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Crate.h"
#include "AmmoCrate.generated.h"

UCLASS()
class NECOGOBLIN_API AAmmoCrate : public ACrate {
	GENERATED_BODY()

private:
	FTimerHandle OnAmmoRegenHandler;
	const int MAX_AMMO = 30 * 3;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Mesh)
	class UStaticMesh* Mesh;
	class UStaticMeshComponent* MeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int AmmoRegenAmount = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	float AmmoRegenRate = 15.f;

	int AmmoAvailable = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UUserWidget> InteractHudWidgetClass;
	UUserWidget* InteractHudWidget;

	void RegenBullets();

public:
	AAmmoCrate();

	void OnWithinRange(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	void OnOutsideRange(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) override;

	UFUNCTION(BlueprintCallable) int GetAmmoAvailable();
};
