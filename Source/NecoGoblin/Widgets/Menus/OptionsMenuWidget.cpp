// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionsMenuWidget.h"
#include "../../Characters/Neco/MainCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "GameFramework/GameUserSettings.h"
#include "Components/WidgetSwitcher.h"
#include "Components/ComboBoxString.h"
#include "Components/Slider.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"

FString UOptionsMenuWidget::IntPointToString(FIntPoint Resolution) {
	return FString(UKismetStringLibrary::Conv_IntToString(Resolution.X) + 'x' + UKismetStringLibrary::Conv_IntToString(Resolution.Y));
}

void UOptionsMenuWidget::InitializeGraphicSlider(USlider* Slider) {
	Slider->SetMaxValue(4.f);
	Slider->SetMinValue(0.f);
}

void UOptionsMenuWidget::InitializeSoundOptions() {

	UGameplayStatics::SetBaseSoundMix(GetWorld(), MenuSoundMix);
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MenuSoundMix, BGM_SC);

	AMainCharacter* mainCharacter = Cast<AMainCharacter>(GetOwningPlayerPawn());
	if (mainCharacter) {
		Options_Sound_Game->SetValue(mainCharacter->GetGameVolume());
		Options_Sound_Music->SetValue(mainCharacter->GetMusicVolume());
	}
}

void UOptionsMenuWidget::InitializeGraphicOptions() {
	TArray<FIntPoint> resolutions;
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(resolutions);
	for (FIntPoint resolution : resolutions) {
		Options_Graphics_Resolution->AddOption(IntPointToString(resolution));
	}
	UGameUserSettings* settings = UGameUserSettings::GetGameUserSettings();
	FIntPoint currentResolution = settings->GetScreenResolution();
	Options_Graphics_Resolution->SetSelectedOption(IntPointToString(currentResolution));

	Options_Graphics_Fullscreen->ClearOptions();
	Options_Graphics_Fullscreen->AddOption(FString(TEXT("Fullscreen")));
	Options_Graphics_Fullscreen->AddOption(FString(TEXT("Borderless")));
	Options_Graphics_Fullscreen->AddOption(FString(TEXT("Windowed")));
	Options_Graphics_Fullscreen->SetSelectedIndex(UKismetMathLibrary::Conv_ByteToInt(settings->GetFullscreenMode()));

	InitializeGraphicSlider(Options_Graphics_AliasingQ);
	Options_Graphics_AliasingQ->SetValue(settings->GetAntiAliasingQuality());
	InitializeGraphicSlider(Options_Graphics_ShadowQ);
	Options_Graphics_ShadowQ->SetValue(settings->GetShadowQuality());
	InitializeGraphicSlider(Options_Graphics_TextureQ);
	Options_Graphics_TextureQ->SetValue(settings->GetTextureQuality());
}

void UOptionsMenuWidget::NativeConstruct() {
	InitializeSoundOptions();
	InitializeGraphicOptions();

	HelpButton->OnClicked.AddDynamic(this, &UOptionsMenuWidget::SwitchHelpMenu);
	OptionsButton->OnClicked.AddDynamic(this, &UOptionsMenuWidget::SwitchOptionMenu);
	QuitButton->OnClicked.AddDynamic(this, &UOptionsMenuWidget::QuitGame);

	Options_GameplayButton->OnClicked.AddDynamic(this, &UOptionsMenuWidget::SwitchOptionGameplay);
	Options_GraphicsButton->OnClicked.AddDynamic(this, &UOptionsMenuWidget::SwitchOptionGraphic);
	Options_SoundButton->OnClicked.AddDynamic(this, &UOptionsMenuWidget::SwitchOptionSound);

	Options_Graphics_Apply->OnClicked.AddDynamic(this, &UOptionsMenuWidget::ApplyGraphics);
	Options_Graphics_Cancel->OnClicked.AddDynamic(this, &UOptionsMenuWidget::CancelGraphics);
	Options_Sound_Apply->OnClicked.AddDynamic(this, &UOptionsMenuWidget::ApplySound);
	Options_Sound_Cancel->OnClicked.AddDynamic(this, &UOptionsMenuWidget::CancelSound);
	Options_Gameplay_Apply->OnClicked.AddDynamic(this, &UOptionsMenuWidget::ApplyGameplay);
	Options_Gameplay_Cancel->OnClicked.AddDynamic(this, &UOptionsMenuWidget::CancelGameplay);

}

void UOptionsMenuWidget::NativeDestruct() {
}

void UOptionsMenuWidget::SwitchHelpMenu() {
	MainSwitcher->SetActiveWidgetIndex((uint8)EOptionMainPanel::VE_Help);
}

void UOptionsMenuWidget::SwitchOptionMenu() {
	MainSwitcher->SetActiveWidgetIndex((uint8)EOptionMainPanel::VE_Option);
}

void UOptionsMenuWidget::QuitGame() {
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void UOptionsMenuWidget::SwitchOptionGameplay() {
	OptionsSwitcher->SetActiveWidgetIndex((uint8)EOptionOptionPanel::VE_Gameplay);
}

void UOptionsMenuWidget::SwitchOptionGraphic() {
	OptionsSwitcher->SetActiveWidgetIndex((uint8)EOptionOptionPanel::VE_Graphics);
}

void UOptionsMenuWidget::SwitchOptionSound() {
	OptionsSwitcher->SetActiveWidgetIndex((uint8)EOptionOptionPanel::VE_Sound);

	AMainCharacter* mainCharacter = Cast<AMainCharacter>(GetOwningPlayerPawn());
	if (mainCharacter) {
		Options_Sound_Game->SetValue(mainCharacter->GetGameVolume());
		Options_Sound_Music->SetValue(mainCharacter->GetMusicVolume());
	}
}

void UOptionsMenuWidget::ApplyGraphics() {
	UGameUserSettings* settings = UGameUserSettings::GetGameUserSettings();

	settings->SetFullscreenMode(static_cast<EWindowMode::Type>(Options_Graphics_Fullscreen->GetSelectedIndex()));
	settings->SetTextureQuality(Options_Graphics_TextureQ->GetValue());
	settings->SetShadowQuality(Options_Graphics_ShadowQ->GetValue());
	settings->SetAntiAliasingQuality(Options_Graphics_AliasingQ->GetValue());
	settings->SetVSyncEnabled(Options_Graphics_Vsync->IsChecked());

	FString selectedResolution = Options_Graphics_Resolution->GetSelectedOption();
	TArray<FString> resolution = UKismetStringLibrary::ParseIntoArray(selectedResolution, FString("x"));
	int32 x = UKismetStringLibrary::Conv_StringToInt(resolution[0]);
	int32 y = UKismetStringLibrary::Conv_StringToInt(resolution[1]);
	settings->SetScreenResolution(FIntPoint(x, y));

	settings->ApplySettings(true);
}

void UOptionsMenuWidget::CancelGraphics() {
	InitializeGraphicOptions();
	MainSwitcher->SetActiveWidgetIndex((uint8)EOptionMainPanel::VE_Info);
}

void UOptionsMenuWidget::ApplySound() {
	AMainCharacter* mainCharacter = Cast<AMainCharacter>(GetOwningPlayerPawn());
	if (mainCharacter) {
		mainCharacter->SetMusicVolume(Options_Sound_Music->GetValue());
		mainCharacter->SetGameVolume(Options_Sound_Game->GetValue());
	}
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MenuSoundMix, BGM_SC, Options_Sound_Music->GetValue());
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MenuSoundMix, SFX_SC, Options_Sound_Game->GetValue());
	MainSwitcher->SetActiveWidgetIndex((uint8)EOptionMainPanel::VE_Info);
}

void UOptionsMenuWidget::CancelSound() {
	AMainCharacter* mainCharacter = Cast<AMainCharacter>(GetOwningPlayerPawn());
	if (mainCharacter) {
		Options_Sound_Music->SetValue(mainCharacter->GetMusicVolume());
		Options_Sound_Game->SetValue(mainCharacter->GetGameVolume());
	}
	MainSwitcher->SetActiveWidgetIndex((uint8)EOptionMainPanel::VE_Info);
}

void UOptionsMenuWidget::ApplyGameplay() {
	AMainCharacter* mainCharacter = Cast<AMainCharacter>(GetOwningPlayerPawn());
	if (mainCharacter) {
		mainCharacter->SetIsToggleAim(Options_Game_Aim->IsChecked());
		mainCharacter->SetIsAutoReload(Options_Game_Reload->IsChecked());
	}
	MainSwitcher->SetActiveWidgetIndex((uint8)EOptionMainPanel::VE_Info);
}

void UOptionsMenuWidget::CancelGameplay() {
	AMainCharacter* mainCharacter = Cast<AMainCharacter>(GetOwningPlayerPawn());
	if (mainCharacter) {
		Options_Game_Aim->SetIsChecked(mainCharacter->GetIsToggleAim());
		Options_Game_Reload->SetIsChecked(mainCharacter->GetIsAutoReload());
	}
	MainSwitcher->SetActiveWidgetIndex((uint8)EOptionMainPanel::VE_Info);
}
