#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

class UButton;
class UVerticalBox;
class USlider;
class UTextBlock;
class ACustomController;

UCLASS(Blueprintable)
class UE5_INTRODUCTION_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	void OpenMenu();

protected:
	UFUNCTION()
	void Resume();
	UFUNCTION()
	void Restart();
	UFUNCTION()
	void Quit();

	UFUNCTION()
	void OpenOptions();
	UFUNCTION()
	void CloseOptions();

	UFUNCTION()
	void ChangeSensitivityX(float value);
	UFUNCTION()
	void ChangeSensitivityY(float value);

protected:
	virtual void NativeConstruct() override;

protected:
	ACustomController* playerController{ nullptr };

	UPROPERTY(meta = (BindWidget))
	UButton* Resume_Button{ nullptr };
	UPROPERTY(meta = (BindWidget))
	UButton* Restart_Button{ nullptr };
	UPROPERTY(meta = (BindWidget))
	UButton* Quit_Button{ nullptr };
	UPROPERTY(meta = (BindWidget))
	UButton* Options_Button{ nullptr };
	UPROPERTY(meta = (BindWidget))
	UButton* Back_Button{ nullptr };

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* Main_VerticalBox{ nullptr };
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* Options_VerticalBox{ nullptr };

	UPROPERTY(meta = (BindWidget))
	USlider* SensitivityX_Slider{ nullptr };
	UPROPERTY(meta = (BindWidget))
	USlider* SensitivityY_Slider{ nullptr };

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SensitivityXValue_Text{ nullptr };
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SensitivityYValue_Text{ nullptr };
};
