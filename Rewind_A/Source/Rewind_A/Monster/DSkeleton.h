// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster.h"
#include "DSkeleton.generated.h"

/**
 * 
 */
UCLASS()
class REWIND_A_API ADSkeleton : public AMonster
{
	GENERATED_BODY()
	

public:
	ADSkeleton();

	virtual void UpdateStats() override;

protected:
	virtual void BeginPlay() override;

};
