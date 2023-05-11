// Fill out your copyright notice in the Description page of Project Settings.


#include "ReGameInstance.h"

UReGameInstance::UReGameInstance()
{
	isFirstLoad = true;
}

void UReGameInstance::setCharacterState(int32 NewPotionCount, int32 NewCHP, float NewRecallUse, float NewControlUse)
{
	PotionCount = NewPotionCount;
	CHP = NewCHP;
	RecallUse = NewRecallUse;
	ControlUse = NewControlUse;
}
