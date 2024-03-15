// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterHUD.generated.h"

/**
 * 
 */
UCLASS()
class NECOGOBLIN_API UCharacterHUD : public UUserWidget {
	GENERATED_BODY()
	
private:

protected:
	UPROPERTY(BlueprintReadWrite)
	class AMainCharacter* MainCharacter;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* Healthbar;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ReserveAmmoCount;

	virtual void NativeConstruct();
	
	UFUNCTION() void UpdateHealth(float Health);

public:
	UFUNCTION() void UpdateReserveAmmo(int ReserveAmmo);
};
