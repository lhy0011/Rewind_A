// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"

#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AMonster::AMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//// 콜리전
	//AttackCollision = CreateDefaultSubobject<USphereComponent>(TEXT("AttackCollision"));
	//AttackCollision->SetupAttachment(RootComponent);
	//AttackCollision->SetSphereRadius(100.0f);
	//AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AIControllerClass = AMonsterAIController::StaticClass(); // 컨트롤러 설정

	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
	bIsDead = false;
}


void AMonster::ChangeState(EMonsterAIState _eNextState, bool _bForce)
{
	if (m_eMState == _eNextState) {
		return;
	}


	m_eMState = _eNextState;

}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();

}

inline void AMonster::OnAttack()
{
	if (AttackMontage)
	{
		PlayAnimMontage(AttackMontage);
	}
	// 각 클래스에 어택 몽타주 설정, 사망 몽타주도
}

void AMonster::TakeMonsterDamage(float Damage)
{
	if (bIsDead) return;

	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealth);
	if (CurrentHealth <= 0.0f)
	{
		bIsDead = true;

		if (DeathMontage)
		{
			PlayAnimMontage(DeathMontage);
		}
	}
}

bool AMonster::IsDead() const
{
	return bIsDead;
}

void AMonster::OnAttackAnimationEnded()
{

}

void AMonster::OnDeathAnimationEnded()
{
	SpawnGem();
	Destroy();
}

void AMonster::SpawnGem()
{
	float SpawnChance = FMath::FRandRange(0.0f, 1.0f);
	if (SpawnChance <= 0.5f && GemActor) // 50% 확률로 보석 스폰
	{
		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = FRotator::ZeroRotator;

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		GetWorld()->SpawnActor<AActor>(GemActor, SpawnLocation, SpawnRotation, SpawnParams);
	}
}