// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class NECOGOBLIN_API UMainMenuWidget : public UUserWidget {
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UUserWidget> SkipIntroWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	UUserWidget* SkipIntroWidget;

	FTimerHandle OnMovieHandler;

	virtual void NativeConstruct() override;

public:
	
	UFUNCTION(BlueprintCallable) void OnStartGame();
	UFUNCTION(BlueprintCallable) void SetGameMode(bool IsEndlessMode);
};
