// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionsMenuWidget.generated.h"

UENUM(BlueprintType)
enum class EOptionMainPanel : uint8 {
	VE_Help UMETA(DisplayName = "Help Menu"),
	VE_Option UMETA(DisplayName = "Option Menu"),
	VE_Info UMETA(DisplayName = "Info Menu"),
};

UENUM(BlueprintType)
enum class EOptionOptionPanel : uint8 {
	VE_Gameplay UMETA(DisplayName = "Gameplay Menu"),
	VE_Graphics UMETA(DisplayName = "Graphics Menu"),
	VE_Sound UMETA(DisplayName = "Sound Menu"),
};

/**
 * 
 */
UCLASS()
class NECOGOBLIN_API UOptionsMenuWidget : public UUserWidget {
	GENERATED_BODY()

private:
	FString IntPointToString(FIntPoint Resolution);
	void InitializeGraphicSlider(class USlider* Slider);

	void InitializeSoundOptions();
	void InitializeGraphicOptions();
	void InitializeGameplayOptions();

protected:
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* HelpButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* OptionsButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* QuitButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UWidgetSwitcher* MainSwitcher;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UWidgetSwitcher* OptionsSwitcher;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UCheckBox* Options_Game_Aim;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UCheckBox* Options_Game_Reload;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Options_Gameplay_Apply;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Options_Gameplay_Cancel;

	UPROPERTY(BLueprintReadWrite, meta = (BindWidget))
	class UButton* Options_GameplayButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Options_GraphicsButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Options_SoundButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UComboBoxString* Options_Graphics_Resolution;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UComboBoxString* Options_Graphics_Fullscreen;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class USlider* Options_Graphics_ShadowQ;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class USlider* Options_Graphics_AliasingQ;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class USlider* Options_Graphics_TextureQ;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UCheckBox* Options_Graphics_Vsync;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Options_Graphics_Apply;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Options_Graphics_Cancel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class USlider* Options_Sound_Game;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class USlider* Options_Sound_Music;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Options_Sound_Apply;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Options_Sound_Cancel;

	UPROPERTY(BlueprintReadWrite)
	class USoundMix* MenuSoundMix;
	UPROPERTY(BlueprintReadWrite)
	class USoundClass* BGM_SC;
	UPROPERTY(BlueprintReadWrite)
	class USoundClass* SFX_SC;

	virtual void NativeConstruct();
	virtual void NativeDestruct();

	UFUNCTION(BlueprintCallable)
	void SwitchHelpMenu();
	UFUNCTION(BlueprintCallable)
	void SwitchOptionMenu();
	UFUNCTION(BlueprintCallable)
	void QuitGame();

	UFUNCTION(BlueprintCallable)
	void SwitchOptionGameplay();
	UFUNCTION(BlueprintCallable)
	void SwitchOptionGraphic();
	UFUNCTION(BlueprintCallable)
	void SwitchOptionSound();

	UFUNCTION(BlueprintCallable)
	void ApplyGraphics();
	UFUNCTION(BlueprintCallable)
	void CancelGraphics();
	UFUNCTION(BlueprintCallable)
	void ApplySound();
	UFUNCTION(BlueprintCallable)
	void CancelSound();
	UFUNCTION(BlueprintCallable)
	void ApplyGameplay();
	UFUNCTION(BlueprintCallable)
	void CancelGameplay();

public:

};
