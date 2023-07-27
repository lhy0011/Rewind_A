// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../Goal/ChallengeManager.h"

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

	UReGameInstance(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Character Data")
		void setCharacterState(int32 NewPotionCount, int32 NewCHP, float NewRecallUse, float NewControlUse, bool NewFGEMGet, bool NewDGEMGet, bool NewIGEMGet, bool NewMGEMGet, bool NewVG, bool NewVD, bool NewVF, bool NewKG, bool NewKD, bool NewKF, bool NewVGP, bool NewVDP, bool NewVFP, bool NewVFIN);
	
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

	UPROPERTY(BlueprintReadWrite, Category = "Character Data")
		bool fGemisGotten;

	UPROPERTY(BlueprintReadWrite, Category = "Character Data")
		bool dGemisGotten;

	UPROPERTY(BlueprintReadWrite, Category = "Character Data")
		bool iGemisGotten;

	UPROPERTY(BlueprintReadWrite, Category = "Character Data")
		bool mGemisGotten;



	UPROPERTY(BlueprintReadWrite, Category = "Character Data")
		bool mVG;

	UPROPERTY(BlueprintReadWrite, Category = "Character Data")
		bool mVD;

	UPROPERTY(BlueprintReadWrite, Category = "Character Data")
		bool mVF;

	UPROPERTY(BlueprintReadWrite, Category = "Character Data")
		bool mKG;

	UPROPERTY(BlueprintReadWrite, Category = "Character Data")
		bool mKD;

	UPROPERTY(BlueprintReadWrite, Category = "Character Data")
		bool mKF;


	UPROPERTY(BlueprintReadWrite, Category = "Character Data")
		bool mVGP;

	UPROPERTY(BlueprintReadWrite, Category = "Character Data")
		bool mVDP;

	UPROPERTY(BlueprintReadWrite, Category = "Character Data")
		bool mVFP;

	UPROPERTY(BlueprintReadWrite, Category = "Character Data")
		bool mVFIN;


	virtual void Init() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MyCategory")
		UChallengeManager* ChallengeManager;


	UFUNCTION(BlueprintCallable, Category = "MyCategory")
		FString GetCurrentChallengeDescription()
	{
		if (ChallengeManager)
		{
			return ChallengeManager->GetCurrentChallengeDescription();
		}
		return TEXT("Clear");
	}
};
