// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Boss.h"

#include "Animation/AnimInstance.h"
#include "B_AnimInst.generated.h"

/**
 * 
 */
UCLASS()
class REWIND_A_API UB_AnimInst : public UAnimInstance
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Info, meta = (AllowPrivateAccess = "true"))
		BMonsterAIState b_eMState;

public:
	virtual void NativeInitializeAnimation() override; // ������ ���� (��ü ���� ��)
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void NativeBeginPlay() override;


	UFUNCTION(BlueprintCallable, Category = "My Category")
		void CallAttackStartNotify();

	UFUNCTION(BlueprintCallable, Category = "My Category")
		void CallAttackEndNotify();
};
