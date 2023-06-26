// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "P_AnimInst.generated.h"

/**
 * 
 */
UCLASS()
class REWIND_A_API UP_AnimInst : public UAnimInstance
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info", meta = (AllowPrivateAccess = "true"))
		EPLAYER_STATE m_eState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info", meta = (AllowPrivateAccess = "true"))
		float						m_Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info", meta = (AllowPrivateAccess = "true"))
		float						m_Dir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info", meta = (AllowPrivateAccess = "true"))
		bool isJ;

public:
	virtual void NativeInitializeAnimation() override; // ������ ���� (��ü ���� ��)
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float _fDT) override;

	UFUNCTION(BlueprintCallable, Category = "My Category")
		void CallCheckNotify();

	UFUNCTION(BlueprintCallable, Category = "My Category")
		void CallEndNotify();

	UFUNCTION(BlueprintCallable, Category = "My Category")
		bool CallIsJumping();

	UFUNCTION(BlueprintCallable, Category = "My Category")
		void EnableCol();

	UFUNCTION(BlueprintCallable, Category = "My Category")
		void DisableCol();

	UFUNCTION(BlueprintCallable)
		void hitEndCall();

	UFUNCTION(BlueprintCallable)
		void RollSCall();

	UFUNCTION(BlueprintCallable)
		void RollECall();

public:
	void SetDirection(float _fDir) { m_Dir = _fDir; }
	float  GetDirection() { return m_Dir; }


	// Notify �� �� �Ʒ���
public:
	UFUNCTION()
		void AnimNotify_AttackEnd();


};
