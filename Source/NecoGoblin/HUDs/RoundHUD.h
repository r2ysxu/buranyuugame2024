// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RoundHUD.generated.h"

/**
 * 
 */
UCLASS()
class NECOGOBLIN_API URoundHUD : public UUserWidget {
	GENERATED_BODY()
	
protected:
	int CurrentRound;
public:

	FORCEINLINE void SetCurrentRound(int Round) { CurrentRound = Round; }
	FORCEINLINE int GetCurrentRound() { return CurrentRound; }

	UFUNCTION(BlueprintCallable) FString GetRoundText();
};
