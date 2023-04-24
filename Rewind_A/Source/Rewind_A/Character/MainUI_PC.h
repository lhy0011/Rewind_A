// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainUI_PC.generated.h"

/**
 * 
 */
UCLASS()
class REWIND_A_API AMainUI_PC : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMainUI_PC();
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UUserWidget> PUIClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		class UUserWidget* PUIObject;

};
