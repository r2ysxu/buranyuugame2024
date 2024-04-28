// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSwitcherMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class NECOGOBLIN_API UCharacterSwitcherMenuWidget : public UUserWidget {
	GENERATED_BODY()
	
protected:
	int SelectedCharacterSkinIndex = 0;
	class AMainCharacter* MainCharacter;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* NextCharacterButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* PrevCharacterButton;

	virtual void NativeConstruct() override;

	UFUNCTION() void OnNextClicked();
	UFUNCTION() void OnPrevClicked();

public:
	void SetParent(class AMainCharacter* Character);
};
