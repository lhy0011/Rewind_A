// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI_PC.h"
#include "../UI/P_MainUI.h"

AMainUI_PC::AMainUI_PC()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MAINUI(TEXT("WidgetBlueprint'/Game/Rewind/UI/HP.HP_C'"));
	if (MAINUI.Succeeded()) {
		PUIClass = MAINUI.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> UIAsset(TEXT("WidgetBlueprint'/Game/Rewind/UI/TimeControlUI.TimeControlUI_C'"));
	if (UIAsset.Succeeded())
	{
		TimeClass = UIAsset.Class;
	}
}

void AMainUI_PC::BeginPlay()
{
	Super::BeginPlay();

	PUIObject = CreateWidget<UUserWidget>(this, PUIClass);
	PUIObject->AddToViewport();

	TimeObject = CreateWidget<UUserWidget>(this, TimeClass);
	//TimeObject->AddToViewport();

	// 원래 UI 저장
	OriginalPUIObject = PUIObject;
}

void AMainUI_PC::ToggleUI(UUserWidget* NewUI)
{
	if (PUIObject != nullptr)
	{
		PUIObject->RemoveFromParent();
	}

	PUIObject = NewUI;

	if (PUIObject != nullptr)
	{
		PUIObject->AddToViewport();
	}
}

void AMainUI_PC::OnToggleUIPressed()
{
	ToggleUI(TimeObject);
}

void AMainUI_PC::OnToggleUIReleased()
{
	ToggleUI(OriginalPUIObject);
}

