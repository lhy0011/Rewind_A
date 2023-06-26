// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "BossAttackCollider.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

#include "Boss.generated.h"

UENUM(BlueprintType)
enum class BMonsterAIState : uint8
{
	Idle,
	Roaming
};

class UB_AnimInst;

UCLASS()
class REWIND_A_API ABoss : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABoss();


private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Info, meta = (AllowPrivateAccess = "true"))
		BMonsterAIState b_eMState;

	UB_AnimInst* b_AnimInst;

public:

	BMonsterAIState GetState() { return b_eMState; }
	void ChangeState(BMonsterAIState _eNextState, bool _bForce = false);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// 체력
	float hp;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* CollisionComponent;

	// 메테오
	UFUNCTION(BlueprintCallable, Category = "Abilities")
		void SummonMeteor();

	UFUNCTION(BlueprintCallable, Category = "Abilities")
		void Earthquake();

	FTimerHandle TimerHandle_EarthquakeAttack;

	FTimerHandle TimerHandle_EarthquakeAttackCooldown;

	FTimerHandle TimerHandle_MeteorAttack;

	FTimerHandle TimerHandle_Attack;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		UAnimMontage* ATMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		UAnimMontage* MTMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		UAnimMontage* EQMontage;



	bool bIsEarthquakeAttackOnCooldown;
	bool bCanSummonMeteor;
	bool bCanAttack;

	void EndEarthquakeCooldown();
	void ResetMeteorCooldown();
	void ResetAttackCooldown();

	float MovementSpeed;

	float MeteorAttackThreshold;
	float EarthquakeAttackThreshold;
	float MeleeAttackThreshold;

	bool isWalking;

	int AttackDamage;


	ABossAttackCollider* Weapon;

	void ActivateAttackCollider();
	void DeactivateAttackCollider();



	UFUNCTION()
		void TakeMonsterDamage(float Damage, AActor* DamageCauser);

	bool bIsDead;

};
