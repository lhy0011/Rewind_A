// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI_PC.h"
#include "../UI/P_MainUI.h"

AMainUI_PC::AMainUI_PC()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MAINUI(TEXT("WidgetBlueprint'/Game/Rewind/UI/HP_C'"));
	if (MAINUI.Succeeded()) {
		PUIClass = MAINUI.Class;
	}
}

void AMainUI_PC::BeginPlay()
{
	Super::BeginPlay();

	PUIObject = CreateWidget<UUserWidget>(this, PUIClass);
	PUIObject->AddToViewport();
}