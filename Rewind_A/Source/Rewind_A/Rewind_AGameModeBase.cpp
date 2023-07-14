// Copyright Epic Games, Inc. All Rights Reserved.


#include "Rewind_AGameModeBase.h"

#include "GameFramework/HUD.h"

#include "Character/P_Character.h"
#include "Character/MainUI_PC.h"

ARewind_AGameModeBase::ARewind_AGameModeBase()
{
    DefaultPawnClass = AP_Character::StaticClass();
    PlayerControllerClass = AMainUI_PC::StaticClass();

    static ConstructorHelpers::FClassFinder<AHUD> PlayerHUDClassFinder(TEXT("Blueprint'/Game/Rewind/UI/MiniMap/Fire/HUD_Fire_MiniMap.HUD_Fire_MiniMap_C'"));
    if (PlayerHUDClassFinder.Succeeded())
    {
        HUDClass = PlayerHUDClassFinder.Class;
    }
}