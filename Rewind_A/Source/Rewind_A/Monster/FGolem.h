// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster.h"
#include "FGolem.generated.h"

/**
 * 
 */
UCLASS()
class REWIND_A_API AFGolem : public AMonster
{
	GENERATED_BODY()

public:
	AFGolem();

protected:
	virtual void BeginPlay() override;

};
