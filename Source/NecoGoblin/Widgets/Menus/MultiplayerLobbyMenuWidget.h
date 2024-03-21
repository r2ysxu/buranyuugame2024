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

	virtual void NativeConstruct() override;
	UFUNCTION() void OnStartGameClicked();
};
