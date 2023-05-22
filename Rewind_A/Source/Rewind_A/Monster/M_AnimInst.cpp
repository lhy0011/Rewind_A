// Fill out your copyright notice in the Description page of Project Settings.


#include "M_AnimInst.h"
#include "GameFramework/Character.h"
#include "Monster.h"

void UM_AnimInst::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}


void UM_AnimInst::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// ���� ������Ʈ�� ���� (�÷��̾�)
	AMonster* pOwner = Cast<AMonster>(TryGetPawnOwner());

	if (nullptr == pOwner)
		return;

	// �÷��̾��� ���°��� ����
	m_eMState = pOwner->GetState();

	// �÷��̾��� ���ǵ�
	//m_eMState = pOwner->GetCharacterMovement()->Velocity.Size();

}

void UM_AnimInst::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UM_AnimInst::CallDeadNotify()
{
	AMonster* Mst = Cast<AMonster>(TryGetPawnOwner());
	if (Mst) {
		Mst->SpawnGem();
		Mst->Destroy();
	}
}

void UM_AnimInst::CallAttackStartNotify()
{
	AMonster* Mst = Cast<AMonster>(TryGetPawnOwner());
	if (Mst) {
		Mst->ActivateAttackCollider();
	}
}

void UM_AnimInst::CallAttackEndNotify()
{
	AMonster* Mst = Cast<AMonster>(TryGetPawnOwner());
	if (Mst) {
		Mst->DeactivateAttackCollider();
	}
}

