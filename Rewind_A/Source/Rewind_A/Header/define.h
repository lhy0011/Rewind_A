#pragma once

UENUM(BlueprintType)
enum class EPLAYER_STATE : uint8
{
	IDLE,
	MOVE,
	ATTACK,
	JUMP,
};
