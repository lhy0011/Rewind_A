// Fill out your copyright notice in the Description page of Project Settings.


#include "ReGameInstance.h"

UReGameInstance::UReGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	isFirstLoad = true;

	ChallengeManager = NewObject<UChallengeManager>(this, TEXT("ChallengeManager"));
}

void UReGameInstance::setCharacterState(int32 NewPotionCount, int32 NewCHP, float NewRecallUse, float NewControlUse, bool NewFGEMGet, bool NewDGEMGet, bool NewIGEMGet, bool NewMGEMGet, bool NewVG, bool NewVD, bool NewVF, bool NewKG, bool NewKD, bool NewKF)
{
	PotionCount = NewPotionCount;
	CHP = NewCHP;
	RecallUse = NewRecallUse;
	ControlUse = NewControlUse;

	fGemisGotten = NewFGEMGet;
	dGemisGotten = NewDGEMGet;
	iGemisGotten = NewIGEMGet;
	mGemisGotten = NewMGEMGet;

	mVG = NewVG;
	mVD = NewVD;
	mVF = NewVG;

	mKG = NewKG;
	mKD = NewKD;
	mKF = NewKF;
}

void UReGameInstance::Init()
{
	Super::Init();

	ChallengeManager->InitializeChallenges();
}


