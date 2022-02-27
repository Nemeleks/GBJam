// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UButton;

UCLASS()
class GBJAM_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* NewGameBtn;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitGameBtn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "New Game Settings")
	FName NewGameLevelName;

	UFUNCTION()
	void NewGame();

	UFUNCTION()
	void QuitGame();
};
