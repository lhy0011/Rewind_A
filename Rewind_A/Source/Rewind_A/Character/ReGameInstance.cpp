// Fill out your copyright notice in the Description page of Project Settings.


#include "ReGameInstance.h"

UReGameInstance::UReGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	isFirstLoad = true;

	ChallengeManager = NewObject<UChallengeManager>(this, TEXT("ChallengeManager"));
}

void UReGameInstance::setCharacterState(int32 NewPotionCount, int32 NewCHP, float NewRecallUse, float NewControlUse, bool NewFGEMGet, bool NewDGEMGet, bool NewIGEMGet, bool NewMGEMGet)
{
	PotionCount = NewPotionCount;
	CHP = NewCHP;
	RecallUse = NewRecallUse;
	ControlUse = NewControlUse;

	fGemisGotten = NewFGEMGet;
	dGemisGotten = NewDGEMGet;
	iGemisGotten = NewIGEMGet;
	mGemisGotten = NewMGEMGet;
}

void UReGameInstance::Init()
{
	Super::Init();

	ChallengeManager->InitializeChallenges();
}


