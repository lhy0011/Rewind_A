// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Challenge.h"

#include "UObject/NoExportTypes.h"
#include "ChallengeManager.generated.h"

/**
 * 
 */
UCLASS()
class REWIND_A_API UChallengeManager : public UObject
{
	GENERATED_BODY()
	
public:
    UChallengeManager(const FObjectInitializer& ObjectInitializer);

    void InitializeChallenges();

    void CheckCurrentChallenge();

   // void UpdateChallenges();

    UFUNCTION(BlueprintCallable, Category = "MyCategory")
    FString GetCurrentChallengeDescription();

private:
    TArray<UChallenge*> Challenges;
    UChallenge* CurrentChallenge;

};
