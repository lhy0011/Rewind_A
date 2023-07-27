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

void UP_AnimInst::CallCheckNotify()
{
	AP_Character* Character = Cast<AP_Character>(TryGetPawnOwner());
	if (Character) {
		Character->ComboAttackCheck();
	}

}

void UP_AnimInst::CallEndNotify()
{
	AP_Character* Character = Cast<AP_Character>(TryGetPawnOwner());
	if (Character) {
		Character->ComboAttackEnd();
	}

}

bool UP_AnimInst::CallIsJumping()
{
	AP_Character* Character = Cast<AP_Character>(TryGetPawnOwner());
	if (Character) {
		isJ=Character->getIsJumping();
	}
	return isJ;
}

void UP_AnimInst::EnableCol()
{
	AP_Character* Character = Cast<AP_Character>(TryGetPawnOwner());
	if (Character) {
		Character->EnableWeaponCollision();
	}
}

void UP_AnimInst::DisableCol()
{
	AP_Character* Character = Cast<AP_Character>(TryGetPawnOwner());
	if (Character) {
		Character->DisableWeaponCollision();
	}
}

void UP_AnimInst::hitEndCall()
{
	AP_Character* Character = Cast<AP_Character>(TryGetPawnOwner());
	if (Character) {
		Character->canAttack = true;
		Character->isComboAttacking = false;
		Character->isHitting = false;
	}
}

void UP_AnimInst::RollSCall()
{
	AP_Character* Character = Cast<AP_Character>(TryGetPawnOwner());
	if (Character) {
		Character->canHit = false;
		Character->isComboAttacking = false;
	}
}

void UP_AnimInst::RollECall()
{
	AP_Character* Character = Cast<AP_Character>(TryGetPawnOwner());
	if (Character) {
		Character->canHit = true;
		Character->isComboAttacking = false;
	}
}

void UP_AnimInst::ESCall()
{
	AP_Character* Character = Cast<AP_Character>(TryGetPawnOwner());
	if (Character) {
		Character->Weapon->NiagaraComponent->SetActive(true);
	}
}

void UP_AnimInst::ECall()
{
	AP_Character* Character = Cast<AP_Character>(TryGetPawnOwner());
	if (Character) {
		Character->Weapon->NiagaraComponent->SetActive(false);
	}
}

void UP_AnimInst::CSATCall()
{
	AP_Character* Character = Cast<AP_Character>(TryGetPawnOwner());
	if (Character) {
		Character->CSAttack();
	}
}

void UP_AnimInst::AnimNotify_AttackEnd()
{
	//UE_LOG(LogTemp, Log, TEXT("Attackend"));

	AP_Character* pOwner = Cast<AP_Character>(TryGetPawnOwner());
	if (nullptr == pOwner) {
		return;
	}

	pOwner->ChangeState(EPLAYER_STATE::IDLE, true);
}
