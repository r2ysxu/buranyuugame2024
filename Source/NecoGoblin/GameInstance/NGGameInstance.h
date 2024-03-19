// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "NGGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class NECOGOBLIN_API UNGGameInstance : public UGameInstance {
	GENERATED_BODY()
	
protected:
	virtual void Init() override;

public:
	UNGGameInstance();
};
