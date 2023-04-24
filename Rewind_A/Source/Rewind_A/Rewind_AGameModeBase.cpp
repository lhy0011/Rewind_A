// Copyright Epic Games, Inc. All Rights Reserved.


#include "Rewind_AGameModeBase.h"

#include "Character/P_Character.h"
#include "Character/MainUI_PC.h"

ARewind_AGameModeBase::ARewind_AGameModeBase()
{
	DefaultPawnClass = AP_Character::StaticClass();
	PlayerControllerClass = AMainUI_PC::StaticClass();
}
