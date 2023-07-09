// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Challenge.generated.h"

/**
 * 
 */
UCLASS()
class REWIND_A_API UChallenge : public UObject
{
	GENERATED_BODY()
	

public:
	UChallenge(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

	virtual bool IsChallengeCompleted() { return false; }

	virtual FString GetChallengeDescription() const { return FString(); }

};
