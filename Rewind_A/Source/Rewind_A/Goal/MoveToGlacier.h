// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../Character/P_Character.h"

#include "Challenge.h"
#include "MoveToGlacier.generated.h"

/**
 * 
 */
UCLASS()
class REWIND_A_API UMoveToGlacier : public UChallenge
{
	GENERATED_BODY()
	
public:
    UMoveToGlacier(const FObjectInitializer& ObjectInitializer);

    virtual bool IsChallengeCompleted() override;

    virtual FString GetChallengeDescription() const override;

private:
    AP_Character* Player;
};
