// Fill out your copyright notice in the Description page of Project Settings.


#include "RoundHUD.h"

FString URoundHUD::GetRoundText() {
	return FString::Printf(TEXT("Round %d"), CurrentRound);
}
