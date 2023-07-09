// Fill out your copyright notice in the Description page of Project Set

#include "MoveToGlacier.h"

UMoveToGlacier::UMoveToGlacier(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // Get the player character
    Player = Cast<AP_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

bool UMoveToGlacier::IsChallengeCompleted()
{
    if (Player)
    {
        FString currentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
        return currentLevelName == "Glacier_out";
    }

    return false;
}

FString UMoveToGlacier::GetChallengeDescription() const
{
	return FString("Move To Ice");
}
