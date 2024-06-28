// Fill out your copyright notice in the Description page of Project Settings.


#include "JoinSessionButton.h"

#include "../../../GameInstance/NGGameInstance.h"
#include "OnlineSessionSettings.h"

UJoinSessionButton::UJoinSessionButton() {}

void UJoinSessionButton::SetupJoinTarget(FOnlineSessionSearchResult JoinableSessionResult) {
	SessionResult = JoinableSessionResult;
	OnClicked.AddDynamic(this, &UJoinSessionButton::OnJoinSession);
}


void UJoinSessionButton::OnJoinSession() {
	Cast<UNGGameInstance>(GetGameInstance())->JoinSearchedSession(SessionResult);
	SetIsEnabled(false);
}
