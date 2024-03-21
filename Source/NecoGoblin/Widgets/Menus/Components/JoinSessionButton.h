// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "OnlineSessionSettings.h"
#include "JoinSessionButton.generated.h"

/**
 * 
 */
UCLASS()
class NECOGOBLIN_API UJoinSessionButton : public UButton {
	GENERATED_BODY()

protected:
	FOnlineSessionSearchResult SessionResult;

	UFUNCTION() void OnJoinSession();

public:
	UJoinSessionButton();

	void SetupJoinTarget(class FOnlineSessionSearchResult JoinableSessionResult);
	
};
