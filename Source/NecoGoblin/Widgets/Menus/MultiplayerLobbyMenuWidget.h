// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MultiplayerLobbyMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class NECOGOBLIN_API UMultiplayerLobbyMenuWidget : public UUserWidget {
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* StartButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* BackButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* NextCharacterButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* PrevCharacterButton;

	int SelectedCharacterSkinIndex = 0;
	class AMainCharacter* MainCharacter;

	virtual void NativeConstruct() override;

	UFUNCTION() void OnStartGameClicked();
	UFUNCTION() void OnNextClicked();
	UFUNCTION() void OnPrevClicked();
	UFUNCTION() void OnBackClicked();

public:
	void SetParent(class AMainCharacter* Character);
};
