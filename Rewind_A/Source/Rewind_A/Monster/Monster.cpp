// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "M_AnimInst.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AMonster::AMonster()
	:m_eMState(EMonsterAIState::Idle)
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
	CurrentHealth = 100.0f;
	bIsDead = false;


	GemActor = AFGem::StaticClass();
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

	m_AnimInst = Cast<UM_AnimInst>(GetMesh()->GetAnimInstance());
	if (m_AnimInst) {

	}
}

inline void AMonster::OnAttack()
{
	if (AttackMontage)
	{
		PlayAnimMontage(AttackMontage);
	}
	// 각 클래스에 어택 몽타주 설정, 사망 몽타주도
}

void AMonster::TakeMonsterDamage(float Damage, AActor* DamageCauser)
{
	//if (bIsDead) return;

	//CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealth);
	//if (CurrentHealth <= 0.0f)
	//{
	//	bIsDead = true;

	//	if (DeathMontage)
	//	{
	//		PlayAnimMontage(DeathMontage);
	//	}
	//}
	UE_LOG(LogTemp, Warning, TEXT("CurrentHealth: %f"), CurrentHealth);

	if (bIsDead)
	{
		return;
	}

	CurrentHealth -= Damage;

	if (Damage > 0.f && DamageCauser) {
		ACharacter* PlayerCharacter = Cast<ACharacter>(DamageCauser);
		if (PlayerCharacter) {
			AMonsterAIController* MonsterAIController = Cast<AMonsterAIController>(GetController());
			if (MonsterAIController) {
				MonsterAIController->OnPlayerDetected(PlayerCharacter);
			}
		}

	}


	if (CurrentHealth <= 0.0f)
	{
		bIsDead = true;
		PlayAnimMontage(DeathMontage);
		SpawnGem();
		Destroy();
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
	if (SpawnChance <= 0.99f && GemActor) // 50% 확률로 보석 스폰
	{
		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = FRotator::ZeroRotator;

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		GetWorld()->SpawnActor<AActor>(GemActor, SpawnLocation, SpawnRotation, SpawnParams);
	}
}

void AMonster::OnMoveCompleted()
{
	ChangeState(EMonsterAIState::Idle);
}

void AMonster::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if (OtherActor && OtherActor->IsA(AWeapon::StaticClass()))
	//{
	//	AWeapon* Weapon = Cast<AWeapon>(OtherActor);
	//	TakeMonsterDamage(Weapon->AttackDamage,Weapon->OwningCharacter);
	//}
}
