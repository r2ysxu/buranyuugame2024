// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "NGGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class NECOGOBLIN_API UNGGameInstance : public UGameInstance {
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegate_SessionFound);
	
protected:

	IOnlineSessionPtr SessionInterface;
	FName DefaultSessionName = FName("Session#");
	TSharedPtr<FOnlineSessionSearch> FoundSessions;
	TArray<FOnlineSessionSearchResult> SessionSearchResults;

	virtual void Init() override;

	void OnCreateSessionComplete(FName SessionName, bool IsSuccessful);
	void OnFindSessionComplete(bool IsSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

public:

	UNGGameInstance();

	FDelegate_SessionFound DelegateSessionFound;

	void HostSession();
	void SearchSessions();
	void JoinSearchedSession(FOnlineSessionSearchResult SessionResult);
	void JumpToLevel(FString LevelName);

	TArray<FOnlineSessionSearchResult> GetSearchResults();
};
