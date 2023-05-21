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

#include "../Character/Weapon.h"
#include "../Item/FGem.h"
#include "../Item/DGem.h"
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
    UFUNCTION(BlueprintImplementableEvent, Category = "Monster")
        inline void OnAttack();

    UFUNCTION()
        void TakeMonsterDamage(float Damage, AActor* DamageCauser);

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

protected:

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


    // 상태 변경 추가

    void ChangeRoaming();
    

    // 타임록 적용

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time Lock")
        float AccumulatedDamage;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time Lock")
        bool bIsTimeLocked;

    // 데미지 누적
    void StoreDamage(float Damage);

    void ApplyAccumulatedDamage();


    // Physics 조정
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
        UPhysicsHandleComponent* PhysicsHandle;

    void StartTimeLock();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
        UStaticMeshComponent* StaticMesh;

    UPROPERTY()
        float StartTimeLockTime;

    UPROPERTY()
        bool bOriginalSimulatePhysics;


    UMaterialInterface* m_OriginalMtrl; // 원래거

    UMaterialInterface* m_TimeLockMtrl; // 타임록 적용

};
