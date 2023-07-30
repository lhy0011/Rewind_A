// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster.h"
#include "GIceGolem.generated.h"

/**
 * 
 */
UCLASS()
class REWIND_A_API AGIceGolem : public AMonster
{
	GENERATED_BODY()
	
public:
	AGIceGolem();

	virtual void UpdateStats() override;

	virtual void TakeMonsterDamage(float Damage, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

	float CSize;
	float OSize;
	float BSize;
	float HSize;

};
