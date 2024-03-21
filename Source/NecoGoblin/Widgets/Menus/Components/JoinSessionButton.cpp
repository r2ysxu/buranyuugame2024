// Fill out your copyright notice in the Description page of Project Settings.

#include "../../../GameInstance/NGGameInstance.h"

#include "JoinSessionButton.h"
#include "OnlineSessionSettings.h"

UJoinSessionButton::UJoinSessionButton() {}

void UJoinSessionButton::SetupJoinTarget(FOnlineSessionSearchResult JoinableSessionResult) {
	SessionResult = JoinableSessionResult;

	OnClicked.AddDynamic(this, &UJoinSessionButton::OnJoinSession);
}


void UJoinSessionButton::OnJoinSession() {
	Cast<UNGGameInstance>(GetGameInstance())->JoinSearchedSession(SessionResult);
}
