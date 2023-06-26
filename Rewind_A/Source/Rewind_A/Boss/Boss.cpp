// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss.h"
#include "Kismet/GameplayStatics.h"
#include "B_AnimInst.h"
#include "../Character/P_Character.h"
#include "Meteor.h"

// Sets default values
ABoss::ABoss()
    :b_eMState(BMonsterAIState::Idle)
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;


    static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(TEXT("SkeletalMesh'/Game/Rewind/Character/FireBoss/FIreBoss'"));
    if (SkeletalMesh.Succeeded())
    {
        GetMesh()->SetSkeletalMesh(SkeletalMesh.Object);
        GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -89.f));
        GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
        GetMesh()->SetWorldScale3D(FVector(4.0f, 4.0f, 4.0f));
    }


    CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
    CollisionComponent->InitCapsuleSize(250.0f, 200.0f);

    CollisionComponent->SetCollisionProfileName(TEXT("OverlapAll"));

    CollisionComponent->SetupAttachment(RootComponent);

    static ConstructorHelpers::FClassFinder<UAnimInstance> BossAnim(TEXT("AnimBlueprint'/Game/Rewind/Character/FireBoss/aim/BossAnimBP.BossAnimBP_C'"));
    if (BossAnim.Succeeded())
    {
        GetMesh()->SetAnimInstanceClass(BossAnim.Class);
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> AMontage(TEXT("AnimMontage'/Game/Rewind/Character/FireBoss/aim/ATMontage'"));
    if (AMontage.Succeeded())
    {
        ATMontage = AMontage.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> MMontage(TEXT("AnimMontage'/Game/Rewind/Character/FireBoss/aim/MTMontage'"));
    if (MMontage.Succeeded())
    {
        MTMontage = MMontage.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> EMontage(TEXT("AnimMontage'/Game/Rewind/Character/FireBoss/aim/EQMontage'"));
    if (EMontage.Succeeded())
    {
        EQMontage = EMontage.Object;
    }


    hp = 1000;

    AttackDamage = 1;

    bIsEarthquakeAttackOnCooldown = false;
    bCanSummonMeteor = false;
    bCanAttack = true;

    MovementSpeed = 180.f;

    MeteorAttackThreshold = 2500.f;
    EarthquakeAttackThreshold = 1500.f;
    MeleeAttackThreshold = 400.f;
}


void ABoss::ChangeState(BMonsterAIState _eNextState, bool _bForce)
{
    if (b_eMState == _eNextState) {
        return;
    }

    b_eMState = _eNextState;
}

// Called when the game starts or when spawned
void ABoss::BeginPlay()
{
    Super::BeginPlay();

    GetWorldTimerManager().SetTimer(TimerHandle_MeteorAttack, this, &ABoss::ResetMeteorCooldown, 5.0f, false);

    b_AnimInst = Cast<UB_AnimInst>(GetMesh()->GetAnimInstance());


    Weapon = GetWorld()->SpawnActor<ABossAttackCollider>(ABossAttackCollider::StaticClass());

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

// Called every frame
void ABoss::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);


    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (PlayerPawn)
    {

        float Distance = FVector::Distance(GetActorLocation(), PlayerPawn->GetActorLocation());

        // 일반공격
        if (Distance <= MeleeAttackThreshold && bCanAttack)
        {
            UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
            if (AnimInstance && ATMontage)
            {
                AnimInstance->Montage_Play(ATMontage, 0.5f);
            }
            bCanAttack = false;
            GetWorld()->GetTimerManager().SetTimer(TimerHandle_Attack, this, &ABoss::ResetAttackCooldown, 6.0f, false);
        }

        // 지진공격
        else if (Distance > MeleeAttackThreshold && Distance <= EarthquakeAttackThreshold && !bIsEarthquakeAttackOnCooldown)
        {

            UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
            if (AnimInstance && EQMontage)
            {
                AnimInstance->Montage_Play(EQMontage, 0.25f);
            }

            // 지진
            Earthquake();

            // 쿨타임
            bIsEarthquakeAttackOnCooldown = true;
            GetWorld()->GetTimerManager().SetTimer(TimerHandle_EarthquakeAttackCooldown, this, &ABoss::EndEarthquakeCooldown, 9.0f, false);
        }

        // 메테오
        else if (Distance > MeteorAttackThreshold && bCanSummonMeteor)
        {
            UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
            if (AnimInstance && EQMontage)
            {
                AnimInstance->Montage_Play(MTMontage, 1.2f);
            }
            SummonMeteor();

        }

        if (Distance < 200) {
            if (b_eMState == BMonsterAIState::Roaming) {
                ChangeState(BMonsterAIState::Idle);
            }
        }

        else // 플레이어에게 이동
        {

            FVector BossLocation = GetActorLocation();
            FVector PlayerLocation = PlayerPawn->GetActorLocation();

            FVector LookDirection = PlayerLocation - BossLocation;
            LookDirection.Z = 0.0f;
            LookDirection.Normalize();

            // 보스 몬스터의 회전 설정
            SetActorRotation(LookDirection.Rotation());

            if (b_eMState != BMonsterAIState::Roaming) {
                ChangeState(BMonsterAIState::Roaming);
            }
            FVector Direction = (PlayerPawn->GetActorLocation() - GetActorLocation()).GetSafeNormal();
            FVector NewLocation = GetActorLocation() + Direction * MovementSpeed * DeltaTime;
            SetActorLocation(NewLocation);
        }
    }

}

// Called to bind functionality to input
void ABoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABoss::SummonMeteor()
{
    if (bCanSummonMeteor) {
        APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        if (PlayerPawn)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

            FVector SpawnLocation = GetActorLocation() + FVector(0, 0, 1200);
            FRotator SpawnRotation = (PlayerPawn->GetActorLocation() - SpawnLocation).Rotation();
            AMeteor* Meteor = GetWorld()->SpawnActor<AMeteor>(AMeteor::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);

            bCanSummonMeteor = false;
            GetWorldTimerManager().SetTimer(TimerHandle_MeteorAttack, this, &ABoss::ResetMeteorCooldown, 5.0f, false);
        }
    }

}

void ABoss::Earthquake()
{
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (PlayerPawn)
    {
        UGameplayStatics::ApplyDamage(PlayerPawn, 10.f, GetController(), this, UDamageType::StaticClass());

        AP_Character* Character = Cast<AP_Character>(PlayerPawn);
        if (Character) {
            Character->TakeDamage(1);
        }
    }
}

void ABoss::EndEarthquakeCooldown()
{
    bIsEarthquakeAttackOnCooldown = false;
}

void ABoss::ResetMeteorCooldown()
{
    //UE_LOG(LogTemp, Warning, TEXT("Reset mt"));
    bCanSummonMeteor = true;
}

void ABoss::ResetAttackCooldown()
{
    bCanAttack = true;
}

void ABoss::ActivateAttackCollider()
{
    if (Weapon) {
        Weapon->EnableCollision();
    }
}

void ABoss::DeactivateAttackCollider()
{
    if (Weapon) {
        Weapon->DisableCollision();
    }
}

void ABoss::TakeMonsterDamage(float Damage, AActor* DamageCauser)
{
    if (bIsDead)
    {
        return;
    }

    hp -= Damage;


}
