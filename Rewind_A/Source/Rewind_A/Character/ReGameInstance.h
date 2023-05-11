// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ReGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class REWIND_A_API UReGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UReGameInstance();

	UFUNCTION(BlueprintCallable, Category = "Character Data")
		void setCharacterState(int32 NewPotionCount, int32 NewCHP, float NewRecallUse, float NewControlUse);
	
	UPROPERTY(BlueprintReadWrite, Category = "Character Data")
		int32 PotionCount;

	UPROPERTY(BlueprintReadWrite, Category = "Character Data")
		int32 CHP;

	UPROPERTY(BlueprintReadWrite, Category = "Character Data")
		float RecallUse;

	UPROPERTY(BlueprintReadWrite, Category = "Character Data")
		float ControlUse;

	UPROPERTY(BlueprintReadWrite, Category = "Character Data")
		bool isFirstLoad;
};
