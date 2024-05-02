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
	TArray<FOnlineSessionSearchResult*> SessionSearchResults;
	float GameVolume = 1.f;
	float MusicVolume = 1.f;

	bool IsToggleAim = false;
	bool IsAutoReload = false;

	virtual void Init() override;

	void OnCreateSessionComplete(FName SessionName, bool IsSuccessful);
	void OnFindSessionComplete(bool IsSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnSessionFailed(const FUniqueNetId& Id, ESessionFailure::Type ErrorType);

public:

	UNGGameInstance();

	FDelegate_SessionFound DelegateSessionFound;

	void HostSession();
	void SearchSessions();
	void JoinSearchedSession(FOnlineSessionSearchResult SessionResult);
	void JumpToLevel(FString LevelName);
	TArray<FOnlineSessionSearchResult*> GetSearchResults();

	UFUNCTION(BlueprintCallable) FORCEINLINE float GetGameVolume() { return GameVolume; }
	UFUNCTION(BlueprintCallable) FORCEINLINE void SetGameVolume(float Volume) { GameVolume = Volume; }
	UFUNCTION(BlueprintCallable) FORCEINLINE float GetMusicVolume() { return MusicVolume; }
	UFUNCTION(BlueprintCallable) FORCEINLINE void SetMusicVolume(float Volume) { MusicVolume = Volume; }
	UFUNCTION(BlueprintCallable) FORCEINLINE bool GetIsToggleAim() { return IsToggleAim; }
	UFUNCTION(BlueprintCallable) FORCEINLINE void SetIsToggleAim(bool isToggleAim) { IsToggleAim = isToggleAim; }
	UFUNCTION(BlueprintCallable) FORCEINLINE bool GetIsAutoReload() { return IsAutoReload; }
	UFUNCTION(BlueprintCallable) FORCEINLINE void SetIsAutoReload(bool isAutoReload) { IsAutoReload = isAutoReload; }
};
