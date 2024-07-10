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
	void SetSelectedCharacterText(int SkinIndex);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UUserWidget> SkipIntroWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	UUserWidget* SkipIntroWidget;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* OptionsEndless;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* NextCharButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* PrevCharButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* CharacterText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* CharacterJobText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* OptionEndlessModeText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UWidgetSwitcher* CharacterProfilePics;

	FTimerHandle OnMovieHandler;
	bool IsEndlessMode;

	virtual void NativeConstruct() override;
	UFUNCTION() void NextCharacter();
	UFUNCTION() void PrevCharacter();

public:
	
	UFUNCTION(BlueprintCallable) void OnStartGame();
	UFUNCTION(BlueprintCallable) void OnModeChanged();
	UFUNCTION(BlueprintCallable) float GetMusicVolume();
};
