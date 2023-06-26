// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "M_AnimInst.h"
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


	// 타임록
	AccumulatedDamage = 0.0f;
	bIsTimeLocked = false;

	bOriginalSimulatePhysics = true;
	StartTimeLockTime = 0.0f;

	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));

	KnockbackIntensity = 1000;


	

	//// 머테리얼
	//m_OriginalMtrl = StaticMesh->GetMaterial(0);

	//ConstructorHelpers::FObjectFinder<UMaterial> mtrl(TEXT("Material'/Game/StarterContent/Materials/M_Metal_Copper'"));
	//if (mtrl.Succeeded())
	//{
	//	m_TimeLockMtrl = mtrl.Object;
	//}
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


	// 소켓 장착
	Weapon = GetWorld()->SpawnActor<AMonsterAttackCollider>(AMonsterAttackCollider::StaticClass());

	if (Weapon) {
		FName SocketName = TEXT("AttackSocket");
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);

		FVector RelativeLocation = FVector(0.0f, 0.0f, 0.0f);
		FRotator RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
		FVector RelativeScale = FVector(1.0f, 1.0f, 1.0f);
		//// 세부 조정

		Weapon->OwningCharacter = this;
	}
}

void AMonster::Tick(float DeltaTime)
{
	//UE_LOG(LogTemp, Warning, TEXT("%f"), CurrentHealth);
}

//inline void AMonster::OnAttack()
//{
//	if (AttackMontage)
//	{
//		PlayAnimMontage(AttackMontage);
//	}
//	// 각 클래스에 어택 몽타주 설정, 사망 몽타주도
//}

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


		if (HitMontage)
		{
			// 캐릭터 쪽으로 돌기
			FVector Direction = PlayerCharacter->GetActorLocation() - GetActorLocation();
			Direction.Normalize();
			FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
			NewLookAt.Pitch = 0.0f;
			NewLookAt.Roll = 0.0f;
			SetActorRotation(NewLookAt);

			PlayAnimMontage(HitMontage);
		}

		//뒤로 물러나기
		FVector KnockbackDirection = GetActorLocation() - DamageCauser->GetActorLocation();
		KnockbackDirection.Normalize();
		LaunchCharacter(KnockbackDirection * KnockbackIntensity, true, true);
	}


	if (CurrentHealth <= 0.0f)
	{
		bIsDead = true;
		//PlayAnimMontage(DeathMontage);

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance) {
			UE_LOG(LogTemp, Warning, TEXT("DeathMontage"));
			AnimInstance->Montage_Play(DeathMontage, 1.0f);

			SpawnGem();

			Destroy();
		}
		

		//SpawnGem();


		//Destroy();
	}

}

void AMonster::MosterAttack()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance) {
		UE_LOG(LogTemp, Warning, TEXT("AttackMontage"));
		AnimInstance->Montage_Play(AttackMontage, 1.0f);
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

void AMonster::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}


void AMonster::ChangeAge(float Value)
{
	Age = Value;
	UE_LOG(LogTemp, Warning, TEXT("Monster age: %f"), Age);

	UpdateStats();
}

void AMonster::UpdateStats()
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Age * 2.0f, 20.0f, 300.0f);

	float newScale = 0.3f + Age * 0.005f;
	GetMesh()->SetWorldScale3D(FVector(newScale));
}

void AMonster::ChangeRoaming()
{
	ChangeState(EMonsterAIState::Roaming);
}

void AMonster::StoreDamage(float Damage)
{
	AccumulatedDamage += Damage;
}

void AMonster::ApplyAccumulatedDamage()
{

	CurrentHealth -= AccumulatedDamage;
	GetMesh()->GlobalAnimRateScale = 1.0f;

	//StaticMesh->SetMaterial(0, m_OriginalMtrl);

	AMonsterAIController* AIController = Cast<AMonsterAIController>(GetController());
	if (AIController)
	{
		ACharacter* PlayerPawn = Cast<ACharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
		if (PlayerPawn)
		{
			AIController->OnPlayerDetected(PlayerPawn);
		}
	}

	// 데미지 리셋
	AccumulatedDamage = 0.0f;
}

void AMonster::StartTimeLock()
{
	// 머테리얼 변화
	//StaticMesh->SetMaterial(0, m_TimeLockMtrl);

	AMonsterAIController* AIController = Cast<AMonsterAIController>(GetController());

	if (AIController)
	{
		GetMesh()->GlobalAnimRateScale = 0.0f;
		AIController->StopMovement();
	}
}

void AMonster::ActivateAttackCollider()
{
	if (Weapon) {
		Weapon->EnableCollision();
	}
}

void AMonster::DeactivateAttackCollider()
{
	if (Weapon) {
		Weapon->DisableCollision();
	}
}

void AMonster::setOld()
{
	GetMesh()->SetSkeletalMesh(OldMesh);

	GetMesh()->SetAnimInstanceClass(OldAnimation);

	DeathMontage = ODeathMontage;
	HitMontage = OHitMontage;
	AttackMontage = OAttackMontage;

}

void AMonster::setChild()
{
	GetMesh()->SetSkeletalMesh(ChildMesh);


	GetMesh()->SetAnimInstanceClass(ChildAnimation);

	DeathMontage = CDeathMontage;
	HitMontage = CHitMontage;
	AttackMontage = CAttackMontage;
}

void AMonster::setBaby()
{
	GetMesh()->SetSkeletalMesh(BabyMesh);


	GetMesh()->SetAnimInstanceClass(BabyAnimation);

	DeathMontage = BDeathMontage;
	HitMontage = BHitMontage;
	AttackMontage = BAttackMontage;
}

