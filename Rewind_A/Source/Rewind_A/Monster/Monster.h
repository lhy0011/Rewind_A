// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/SphereComponent.h"
#include "MonsterAIController.h"
#include "Animation/AnimMontage.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

#include "MonsterAttackCollider.h"
#include "../Character/Weapon.h"
#include "../Item/FGem.h"
#include "../Item/DGem.h"
#include "../Item/GGem.h"
#include "GameFramework/Character.h"
#include "Monster.generated.h"

UENUM(BlueprintType)
enum class EMonsterAIState : uint8
{
    Idle,
    Roaming,
    Attacking
};

class UM_AnimInst;

UCLASS()
class REWIND_A_API AMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonster();

private:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Info, meta = (AllowPrivateAccess = "true"))
        EMonsterAIState m_eMState;

    UM_AnimInst* m_AnimInst;

public:

    EMonsterAIState GetState() { return m_eMState; }
    void ChangeState(EMonsterAIState _eNextState, bool _bForce = false);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
    virtual void Tick(float DeltaTime) override;

//
//	// Called to bind functionality to input
//	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:
    //UFUNCTION(BlueprintImplementableEvent, Category = "Monster")
    //    inline void OnAttack();

    UFUNCTION()
        void TakeMonsterDamage(float Damage, AActor* DamageCauser);


    UFUNCTION()
        void MosterAttack();

    bool IsDead() const;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
        class UCapsuleComponent* CollisionComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
        UAnimMontage* AttackMontage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
        UAnimMontage* DeathMontage;

    // Add these functions
    UFUNCTION(BlueprintCallable, Category = "Animation")
        void OnAttackAnimationEnded();

    UFUNCTION(BlueprintCallable, Category = "Animation")
        void OnDeathAnimationEnded();

    //UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loot")
    //    TSubclassOf<AActor> GemActor;

    // Add this function
    UFUNCTION(BlueprintCallable, Category = "Loot")
        void SpawnGem();

    UPROPERTY(EditAnywhere, Category = "Spawning")
        TSubclassOf<class AFGem> GemActor;

public:

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster")
        float MaxHealth;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster")
        float CurrentHealth;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster")
        int AttackDamage;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster")
        bool bIsDead;

public:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster")
        float Age;

    UFUNCTION()
        void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    
    void ChangeAge(float Value);

    virtual void UpdateStats();


    // ���� ���� �߰�

    void ChangeRoaming();
    

    // Ÿ�ӷ� ����

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time Lock")
        float AccumulatedDamage;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time Lock")
        bool bIsTimeLocked;

    // ������ ����
    void StoreDamage(float Damage);

    void ApplyAccumulatedDamage();


    // Physics ����
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
        UPhysicsHandleComponent* PhysicsHandle;

    void StartTimeLock();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
        UStaticMeshComponent* StaticMesh;

    UPROPERTY()
        float StartTimeLockTime;

    UPROPERTY()
        bool bOriginalSimulatePhysics;


    UMaterialInterface* m_OriginalMtrl; // ������

    UMaterialInterface* m_TimeLockMtrl; // Ÿ�ӷ� ����

    // ���� �ݸ���
    AMonsterAttackCollider* Weapon;

    void ActivateAttackCollider();
    void DeactivateAttackCollider();


    // �ǰ�
    UPROPERTY(EditAnywhere, Category = "Animation")
        class UAnimMontage* HitMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
        float KnockbackIntensity;




    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
        class USpringArmComponent* SpringArm;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
        class UCameraComponent* Camera;




    class USkeletalMesh* OldMesh;
    TSubclassOf<class UAnimInstance> OldAnimation;
    UAnimMontage* ODeathMontage;
    UAnimMontage* OAttackMontage;
    UAnimMontage* OHitMontage;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Change")
    class USkeletalMesh* ChildMesh;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Change")
        TSubclassOf<class UAnimInstance> ChildAnimation;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
        UAnimMontage* CDeathMontage;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
        UAnimMontage* CAttackMontage;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
        UAnimMontage* CHitMontage;


    class USkeletalMesh* BabyMesh;
    TSubclassOf<class UAnimInstance> BabyAnimation;
    UAnimMontage* BDeathMontage;
    UAnimMontage* BAttackMontage;
    UAnimMontage* BHitMontage;

    void setOld();
    void setChild();
    void setBaby();
    




};
