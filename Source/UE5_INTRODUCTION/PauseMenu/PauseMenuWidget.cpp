#include "PauseMenu/PauseMenuWidget.h"
#include "Controller/CustomController.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "Defines.h"

void UPauseMenuWidget::OpenMenu()
{
	if (!IsValid(playerController)) return;

	UGameplayStatics::SetGamePaused(GetWorld(), true);

	SetVisibility(ESlateVisibility::Visible);

	playerController->SetShowMouseCursor(true);

	playerController->SetInputMode(FInputModeUIOnly{});
}

void UPauseMenuWidget::Resume()
{
	if (!IsValid(playerController)) return;

	playerController->SetShowMouseCursor(false);

	SetVisibility(ESlateVisibility::Hidden);

	UGameplayStatics::SetGamePaused(GetWorld(), false);

	playerController->SetInputMode(FInputModeGameOnly{});
}

void UPauseMenuWidget::Restart()
{
	if (!IsValid(playerController)) return;

	playerController->SetInputMode(FInputModeGameOnly{});

	FName level_name = FName(UGameplayStatics::GetCurrentLevelName(this));
	UGameplayStatics::OpenLevel(this, level_name);
}

void UPauseMenuWidget::Quit()
{
	if (!IsValid(playerController)) return;

	UKismetSystemLibrary::QuitGame(this, playerController, EQuitPreference::Quit, true);
}

void UPauseMenuWidget::OpenOptions()
{
	if (!IsValid(playerController)) return;

	Main_VerticalBox->SetVisibility(ESlateVisibility::Collapsed);
	Options_VerticalBox->SetVisibility(ESlateVisibility::Visible);
}

void UPauseMenuWidget::CloseOptions()
{
	if (!IsValid(playerController)) return;

	Main_VerticalBox->SetVisibility(ESlateVisibility::Visible);
	Options_VerticalBox->SetVisibility(ESlateVisibility::Collapsed);
}

void UPauseMenuWidget::ChangeSensitivityX(float value)
{
	if (!IsValid(playerController)) return;

	SensitivityXValue_Text->SetText(UKismetTextLibrary::Conv_FloatToText(value, ERoundingMode::HalfToEven, false, true, 1, 1, 1, 1));

	playerController->SetMouseSensitivityX(value);
}

void UPauseMenuWidget::ChangeSensitivityY(float value)
{
	if (!IsValid(playerController)) return;

	SensitivityYValue_Text->SetText(UKismetTextLibrary::Conv_FloatToText(value, ERoundingMode::HalfToEven, false, true, 1, 1, 1, 1));

	playerController->SetMouseSensitivityY(value);
}

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	playerController = Cast<ACustomController>(UGameplayStatics::GetPlayerController(this, 0));
	if (!IsValid(playerController)) return;

	Main_VerticalBox->SetVisibility(ESlateVisibility::Visible);
	Options_VerticalBox->SetVisibility(ESlateVisibility::Collapsed);

	SensitivityX_Slider->SetValue(playerController->GetMouseSensitivityX());
	ChangeSensitivityX(playerController->GetMouseSensitivityX());
	SensitivityY_Slider->SetValue(playerController->GetMouseSensitivityY());
	ChangeSensitivityY(playerController->GetMouseSensitivityY());

	Resume_Button->OnClicked.AddUniqueDynamic(this, &UPauseMenuWidget::Resume);
	Restart_Button->OnClicked.AddUniqueDynamic(this, &UPauseMenuWidget::Restart);
	Quit_Button->OnClicked.AddUniqueDynamic(this, &UPauseMenuWidget::Quit);
	Options_Button->OnClicked.AddUniqueDynamic(this, &UPauseMenuWidget::OpenOptions);
	Back_Button->OnClicked.AddUniqueDynamic(this, &UPauseMenuWidget::CloseOptions);

	SensitivityX_Slider->OnValueChanged.AddUniqueDynamic(this, &UPauseMenuWidget::ChangeSensitivityX);
	SensitivityY_Slider->OnValueChanged.AddUniqueDynamic(this, &UPauseMenuWidget::ChangeSensitivityY);

	OpenMenu();
}
