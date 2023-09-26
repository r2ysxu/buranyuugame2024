// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UpgradeSkillComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NECOGOBLIN_API UUpgradeSkillComponent : public UActorComponent {
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UUserWidget* ShopHudWidget;

	class AMainCharacter* ParentCharacter;
	int Points = 0;
	bool CanShop = false;

public:	

	// Sets default values for this component's properties
	UUpgradeSkillComponent();

	void SetParentCharacter(AMainCharacter* mainCharacter);

	UFUNCTION(BlueprintCallable) void UpgradeWeaponDamage(int points, float additionalDamage);
	UFUNCTION(BlueprintCallable) void UpgradeHealth(int points, int additionalHealth);
	UFUNCTION(BlueprintCallable) void HealToFull(int points);

	UFUNCTION(BlueprintCallable) FORCEINLINE int GetPoints() { return Points; }
	FORCEINLINE bool GetCanShop() { return CanShop; }
	void SetCanShop(bool canShop);
	void EnterShopScreen(UUserWidget* shopClassWidget);
	UFUNCTION(BlueprintCallable) void ExitShopScreen();

	bool IncrementPoints(int points);
	bool DecrementPoints(int points);
};
