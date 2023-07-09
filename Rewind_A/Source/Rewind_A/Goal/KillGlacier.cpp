// Fill out your copyright notice in the Description page of Project Settings.


#include "KillGlacier.h"

UKillGlacier::UKillGlacier(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // Get the player character
    Player = Cast<AP_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

bool UKillGlacier::IsChallengeCompleted()
{
    if (Player)
    {
        return Player->isKillIce;
    }

    return false;
}

FString UKillGlacier::GetChallengeDescription() const
{
	return FString("Kill Ice Monster");
}
