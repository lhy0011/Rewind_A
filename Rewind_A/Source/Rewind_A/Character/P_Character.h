// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"

#include "CoreMinimal.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Async/Async.h"

#include "GameFramework/Character.h"
#include "P_Character.generated.h"

class UP_AnimInst;
class AAfterIMG;
class AInteractableItem;
class AP_Potion;

UCLASS()
class REWIND_A_API AP_Character : public ACharacter
{
	GENERATED_BODY()


private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* m_pCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* m_pSpringArm;

	//����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Info, meta = (AllowPrivateAccess = "true"))
		EPLAYER_STATE m_eState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Info, meta = (AllowPrivateAccess = "true"))
		TArray<UAnimMontage*> m_arrMontage;



	//�ݸ���
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* CollisionCapsule;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 otherBodyIndex, bool bRfromSweep, const FHitResult& Sweep);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);





	// AnimInst�� �ּҰ��� �޴� ����
	UP_AnimInst* m_AnimInst;

	// SaveLocation ��ġ ���� �迭
	TArray<FVector> S_Location1;
	TArray<FVector> S_Location2;
	TArray<FVector> S_Location3;
	TArray<FVector> S_Location4;

	FVector RecallLocation[10]; // Ÿ�� ���� �Լ� 
	int countRtime = 0; // 1�ʸ��� ++
	int CRT = 0; // ������ �����ϴ� ���� 

	FTimerHandle MyTimerHandle;

	void RecallCountNsave();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info", meta = (AllowPrivateAccess = "true"))
		int CHP;

	UFUNCTION(BlueprintCallable)
		int getCHP() const { return CHP; }

	// ����
	int CPotion;

	UFUNCTION(BlueprintCallable)
		int getCPotion() const { return CPotion; }



public:
	EPLAYER_STATE GetState() { return m_eState; }
	void ChangeState(EPLAYER_STATE _eNextState, bool _bForce = false);

	// ������ ����
	UFUNCTION(BlueprintCallable, Category = "Health")
		void TakeDamage(float DamageAmount);

	UFUNCTION(BlueprintCallable, Category = "Health")
		void UseHealP();


	// ������ ��ȣ�ۿ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
		float InteractionDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
		AInteractableItem* CurrentInteractableItem;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
		void EnableInteraction(AInteractableItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
		void DisableInteraction();

	UFUNCTION()
		void Interact();

	// �ƿ����� ���� ����
	UPROPERTY()
		AInteractableItem* PreviousOutlinedItem;






public:
	// Sets default values for this character's properties
	AP_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



	// ������
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:



	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info", meta = (AllowPrivateAccess = "true"))
		float CDamage;

private:
	int SCHP[10] = { 0 }; // ü�� ���庯��
	FVector SCL[5] = {};


public:
	void CharacterMoveFront(float _fScale);
	void CharacterMoveRight(float _fScale);
	void CharacterRotationZ(float _fScale);
	void CharacterRotationY(float _fScale);

	void CharacterTimeControl();
	void CharacterTimeControlB(); // Ű�� ���� �� �ǵ����°�
	void CharacterTimeRecall();
	void CharacterTPL1();
	void CharacterTPL2();
	void CharacterTPL3();
	void CharacterTPL4();
	void CharacterSaveLocation();
	void CharacterTPLocation(int SN);
	void CharacterJump();

	void SaveCurPose();



	// �޺� ����
	public:
		UPROPERTY(EditAnywhere, Category = "Animation")
			class UAnimMontage* AttackMontage;

		bool isComboAttacking;
		bool isComboAttackDown;
		bool isComboAttackNext;
		int ComboAttackCount;

		UFUNCTION(BlueprintCallable)
			void ComboAttackDown();

		UFUNCTION(BlueprintCallable)
			void ComboAttackUp();

		UFUNCTION(BlueprintCallable)
			void ComboAttack();

		UFUNCTION(BlueprintCallable)
			void ComboAttackEnd();

		UFUNCTION(BlueprintCallable)
			void ComboAttackCheck();




private:
	void SaveCurLocation();

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	//	UAnimMontage* AttackMontage;


	//cooldown

	float RecallCooldown = 1.f;
	float TimeControlCool = 1.f;

	float RecallUse;
	float ControlUse;

	bool canRecall = true;
	bool CanControl = true;

	float tp1, tp2, tp3, tp4;
	//bool canTP1, canTP2, canTP3, canTP4 = false;

	AAfterIMG* tp1AIMG;
	AAfterIMG* tp2AIMG;
	AAfterIMG* tp3AIMG;
	AAfterIMG* tp4AIMG;

	// Ÿ�̸� ����
	FTimerHandle TimerHandle;


	UFUNCTION(BlueprintCallable)
		float getRecallUse() const { return RecallUse; }

	UFUNCTION(BlueprintCallable)
		float getControlUse() const { return ControlUse; }

	UFUNCTION(BlueprintCallable)
		float getTP1() const { return tp1; }

	UFUNCTION(BlueprintCallable)
		float getTP2() const { return tp2; }

	UFUNCTION(BlueprintCallable)
		float getTP3() const { return tp3; }

	UFUNCTION(BlueprintCallable)
		float getTP4() const { return tp4; }



	// ������
	bool isJumping;


	// UI

	UPROPERTY(VisibleAnywhere, Category = UI)
		class UWidgetComponent* HPWidget;

	UPROPERTY(EditAnywhere, Category = UI)
		TSubclassOf<UUserWidget> WidgetClass;






};
