// Fill out your copyright notice in the Description page of Project Settings.


#include "B_AnimInst.h"
#include "Boss.h"

void UB_AnimInst::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UB_AnimInst::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	ABoss* pOwner = Cast<ABoss>(TryGetPawnOwner());

	if (nullptr == pOwner)
		return;

	// 플레이어의 상태값을 갱신
	b_eMState = pOwner->GetState();
}

void UB_AnimInst::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UB_AnimInst::CallAttackStartNotify()
{
	ABoss* Mst = Cast<ABoss>(TryGetPawnOwner());
	if (Mst) {
		Mst->ActivateAttackCollider();
	}
}

void UB_AnimInst::CallAttackEndNotify()
{
	ABoss* Mst = Cast<ABoss>(TryGetPawnOwner());
	if (Mst) {
		Mst->DeactivateAttackCollider();
	}
}
