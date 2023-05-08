// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/SphereComponent.h"
#include "MonsterAIController.h"
#include "Animation/AnimMontage.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

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

    EMonsterAIState GetState() { return m_eMState; }
    void ChangeState(EMonsterAIState _eNextState, bool _bForce = false);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

//public:	
//	// Called every frame
//	virtual void Tick(float DeltaTime) override;
//
//	// Called to bind functionality to input
//	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:
    UFUNCTION(BlueprintImplementableEvent, Category = "Monster")
        inline void OnAttack();

    UFUNCTION()
        void TakeMonsterDamage(float Damage);

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

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loot")
        TSubclassOf<AActor> GemActor;

    // Add this function
    UFUNCTION(BlueprintCallable, Category = "Loot")
        void SpawnGem();

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
    void OnMoveCompleted();


private:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Info, meta = (AllowPrivateAccess = "true"))
        EMonsterAIState m_eMState;

    UM_AnimInst* m_AnimInst;

};
