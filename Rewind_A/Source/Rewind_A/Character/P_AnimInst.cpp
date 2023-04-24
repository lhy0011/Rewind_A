// Fill out your copyright notice in the Description page of Project Settings.


#include "P_AnimInst.h"

#include "P_Character.h"

void UP_AnimInst::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UP_AnimInst::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UP_AnimInst::NativeUpdateAnimation(float _fDT)
{
	Super::NativeUpdateAnimation(_fDT);

	// ���� ������Ʈ�� ���� (�÷��̾�)
	AP_Character* pOwner = Cast<AP_Character>(TryGetPawnOwner());

	if (nullptr == pOwner)
		return;

	// �÷��̾��� ���°��� ����
	m_eState = pOwner->GetState();

	// �÷��̾��� ���ǵ�
	m_Speed = pOwner->GetCharacterMovement()->Velocity.Size();

	// �÷��̾��� �̵� ���� -> P_Character.cpp ���� �̵�Ű�� �Է¹��� �� ������ �����ǰ� ��

}

void UP_AnimInst::AnimNotify_AttackEnd()
{
	UE_LOG(LogTemp, Log, TEXT("Attackend"));

	AP_Character* pOwner = Cast<AP_Character>(TryGetPawnOwner());
	if (nullptr == pOwner) {
		return;
	}

	pOwner->ChangeState(EPLAYER_STATE::IDLE, true);
}
