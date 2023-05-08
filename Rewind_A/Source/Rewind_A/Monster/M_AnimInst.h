// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Monster.h"

#include "Animation/AnimInstance.h"
#include "M_AnimInst.generated.h"

/**
 * 
 */
UCLASS()
class REWIND_A_API UM_AnimInst : public UAnimInstance
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Info, meta = (AllowPrivateAccess = "true"))
		EMonsterAIState m_eMState;

public:
	virtual void NativeInitializeAnimation() override; // 积己磊 开且 (按眉 积己 矫)
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

};
