// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartMainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class NECOGOBLIN_API UStartMainMenuWidget : public UUserWidget {
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* PlaySingleButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* PlayMultiplayerButton;

	class AMainMenuActor* Parent;

	virtual void NativeConstruct() override;

	UFUNCTION() void OnPlaySPClicked();
	UFUNCTION() void OnPlayMPClicked();

public:
	UFUNCTION(BlueprintCallable) void SetParent(class AMainMenuActor* MenuActor);
};
