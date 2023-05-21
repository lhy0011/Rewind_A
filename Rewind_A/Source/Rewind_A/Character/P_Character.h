// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"

#include "CoreMinimal.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Async/Async.h"
#include "Weapon.h"
#include "../Item/FGem.h"
#include "../Item/DGem.h"
#include "../Item/TimeLockableActor.h"
#include "../Monster/Monster.h"
#include "MainUI_PC.h"

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

	//상태
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Info, meta = (AllowPrivateAccess = "true"))
		EPLAYER_STATE m_eState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Info, meta = (AllowPrivateAccess = "true"))
		TArray<UAnimMontage*> m_arrMontage;



	//콜리전
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* CollisionCapsule;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 otherBodyIndex, bool bRfromSweep, const FHitResult& Sweep);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);





	// AnimInst의 주소값을 받는 변수
	UP_AnimInst* m_AnimInst;

	// SaveLocation 위치 저장 배열
	TArray<FVector> S_Location1;
	TArray<FVector> S_Location2;
	TArray<FVector> S_Location3;
	TArray<FVector> S_Location4;

	FVector RecallLocation[10]; // 타임 리콜 함수 
	int countRtime = 0; // 1초마다 ++
	int CRT = 0; // 나눠서 저장하는 변수 

	FTimerHandle MyTimerHandle;

	void RecallCountNsave();


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info", meta = (AllowPrivateAccess = "true"))
		int32 CHP;

	UFUNCTION(BlueprintCallable)
		int32 getCHP() const { return CHP; }

	UFUNCTION(BlueprintCallable, Category = "Character Data")
		void setCHP(int32 NewCHP) { CHP = NewCHP; }


	// 포션
	int32 CPotion;

	UFUNCTION(BlueprintCallable)
		int32 getCPotion() const { return CPotion; }

	UFUNCTION(BlueprintCallable, Category = "Character Data")
		void SetPotionCount(int32 NewPotionCount) { CPotion = NewPotionCount; }



	EPLAYER_STATE GetState() { return m_eState; }
	void ChangeState(EPLAYER_STATE _eNextState, bool _bForce = false);

	// 데미지 받음
	UFUNCTION(BlueprintCallable, Category = "Health")
		void TakeDamage(float DamageAmount);

	UFUNCTION(BlueprintCallable, Category = "Health")
		void UseHealP();


	// 아이템 상호작용
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

	// 아웃라인 추적 변수
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



private:
	void SetCamera();
	void SetCollision();
	void SetAsset();

	// 최종뎀
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:



	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info", meta = (AllowPrivateAccess = "true"))
		float CDamage;

private:
	int SCHP[10] = { 0 }; // 체력 저장변수
	FVector SCL[5] = {};


public:
	void CharacterMoveFront(float _fScale);
	void CharacterMoveRight(float _fScale);
	void CharacterRotationZ(float _fScale);
	void CharacterRotationY(float _fScale);

	void CharacterTimeControl();
	void CharacterTimeControlB(); // 키를 뗐을 때 되돌리는거
	void CharacterTimeRecall();
	void CharacterTPL1();
	void CharacterTPL2();
	void CharacterTPL3();
	void CharacterTPL4();
	void CharacterSaveLocation();
	void CharacterTPLocation(int SN);
	void CharacterJump();
	void MoveMain();

	void SaveCurPose();



	// 콤보 공격
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

	bool getisComboAttacking() { return isComboAttacking; }



	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		AWeapon* Weapon;

	UFUNCTION(BlueprintCallable, Category = "Animation")
		void EnableWeaponCollision();

	UFUNCTION(BlueprintCallable, Category = "Animation")
		void DisableWeaponCollision();



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

	// 타이머 변수
	FTimerHandle TimerHandle;

public:
	UFUNCTION(BlueprintCallable)
		float getRecallUse() const { return RecallUse; }

	UFUNCTION(BlueprintCallable)
		float getControlUse() const { return ControlUse; }


	UFUNCTION(BlueprintCallable, Category = "Character Data")
		void setRecallUse(float NewRecallUse) { RecallUse = NewRecallUse; }

	UFUNCTION(BlueprintCallable, Category = "Character Data")
		void seControlUse(float NewControlUse) { RecallUse = NewControlUse; }

	UFUNCTION(BlueprintCallable)
		float getTP1() const { return tp1; }

	UFUNCTION(BlueprintCallable)
		float getTP2() const { return tp2; }

	UFUNCTION(BlueprintCallable)
		float getTP3() const { return tp3; }

	UFUNCTION(BlueprintCallable)
		float getTP4() const { return tp4; }


	UFUNCTION(BlueprintCallable, Category = "Character Data")
		void setCharacterState(int32 NewPotionCount, int32 NewCHP, float NewRecallUse, float NewControlUse, bool NewFGEMGet, bool NewDGEMGet, bool NewIGEMGet, bool NewMGEMGet);



	// 점프중
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info", meta = (AllowPrivateAccess = "true"))
		bool isJumping;

	UFUNCTION(BlueprintCallable)
		bool getIsJumping() { return isJumping; }


	// UI

	UPROPERTY(VisibleAnywhere, Category = UI)
		class UWidgetComponent* HPWidget;

	UPROPERTY(EditAnywhere, Category = UI)
		TSubclassOf<UUserWidget> WidgetClass;


	// 젬 소유
public:
	bool fGemisGotten;
	bool dGemisGotten;
	bool iGemisGotten;
	bool mGemisGotten;

	UFUNCTION(BlueprintCallable)
		bool getfGemisGotten() const { return fGemisGotten; }

	UFUNCTION(BlueprintCallable)
		bool getIsGotFG() const { return fGemisGotten; }

	UFUNCTION(BlueprintCallable)
		bool getIsGotDG() const { return dGemisGotten; }

	UFUNCTION(BlueprintCallable)
		bool getIsGotIG() const { return iGemisGotten; }

	UFUNCTION(BlueprintCallable)
		bool getIsGotMG() const { return mGemisGotten; }


	float DistMonster = 300.0f;



	// 타임컨트롤
	public:
		void OnMouseWheelScroll();

		void OnMouseWheelScrollDown();

		float AgeValue;


		bool isTimeControlling;

		AMonster* NearestMonster;

		void UpdateNearestMonster();

		void AgeMonster(float Value);


		// 구르기
		FTimerHandle RollTimerHandle;

		UPROPERTY(EditAnywhere, Category = "Animation")
			class UAnimMontage* RollMontage;

		UFUNCTION(BlueprintCallable)
			void RollCharacter();

		void RollMove();


		// 타임록
		void UpdateNearestTLActor();

		void OnTimeLock();

		void OnTimeLockEnd();

		ATimeLockableActor* NearestActor1;

		bool isTimeLocking;

		FTimerHandle TimeLockHandle;


		void AATimeLock();
		void MMTimeLock();


		// 치트키
		void DashCheat();
		void DashCheatEnd();
		void exitGame();
		void ResetSkill();
};
