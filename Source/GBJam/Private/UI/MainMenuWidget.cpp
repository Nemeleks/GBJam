// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenuWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (NewGameBtn)	
	{
		NewGameBtn->OnClicked.AddDynamic(this, &ThisClass::UMainMenuWidget::NewGame);
	}
	if (QuitGameBtn)
	{
		QuitGameBtn->OnClicked.AddDynamic(this, &ThisClass::UMainMenuWidget::QuitGame);
	}
}

void UMainMenuWidget::NewGame()
{
	UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
	UGameplayStatics::OpenLevel(GetWorld(), NewGameLevelName);
}

void UMainMenuWidget::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}