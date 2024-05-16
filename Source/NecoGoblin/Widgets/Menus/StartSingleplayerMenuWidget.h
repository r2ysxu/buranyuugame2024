// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartSingleplayerMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class NECOGOBLIN_API UStartSingleplayerMenuWidget : public UUserWidget {
	GENERATED_BODY()

private:
	void SetSelectedCharacterText();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UUserWidget> SkipIntroWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	UUserWidget* SkipIntroWidget;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UCheckBox* Options_Endless;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* NextCharButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* PrevCharButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* CharacterText;

	FTimerHandle OnMovieHandler;
	int SelectedCharacterSkinIndex = 0;

	virtual void NativeConstruct() override;
	UFUNCTION() void NextCharacter();
	UFUNCTION() void PrevCharacter();

public:
	
	UFUNCTION(BlueprintCallable) void OnStartGame();
	UFUNCTION(BlueprintCallable) void SetGameMode(bool IsEndlessMode);
	UFUNCTION(BlueprintCallable) float GetMusicVolume();
};
