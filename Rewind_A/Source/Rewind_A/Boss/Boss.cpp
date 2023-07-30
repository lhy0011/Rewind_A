// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss.h"
#include "Kismet/GameplayStatics.h"
#include "B_AnimInst.h"
#include "../Character/P_Character.h"
#include "Meteor.h"

// Sets default values
ABoss::ABoss()
    :b_eMState(BMonsterAIState22::Idle)
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

    static ConstructorHelpers::FObjectFinder<UAnimMontage> DDMontage(TEXT("AnimMontage'/Game/Rewind/Character/FireBoss/aim/BossDeath'"));
    if (DDMontage.Succeeded())
    {
        DMontage = DDMontage.Object;
    }


    hp = 1000.0f;

    AttackDamage = 1;

    bIsEarthquakeAttackOnCooldown = false;
    bCanSummonMeteor = false;
    bCanAttack = true;

    canWalking = true;

    MovementSpeed = 350.f;

    MeteorAttackThreshold = 2500.f;
    EarthquakeAttackThreshold = 1500.f;
    MeleeAttackThreshold = 400.f;



    static ConstructorHelpers::FObjectFinder<USoundWave> SoundObj(TEXT("SoundWave'/Game/Rewind/BGM/Hit/cutROSEKNIGHT-HEAVE'"));
    if (SoundObj.Succeeded()) {
        HitSound = SoundObj.Object;
    }

    static ConstructorHelpers::FObjectFinder<USoundWave> SoundObj2(TEXT("SoundWave'/Game/Rewind/BGM/FM/dragon_roar09'"));
    if (SoundObj2.Succeeded()) {
        MTSound = SoundObj2.Object;
    }

    static ConstructorHelpers::FObjectFinder<USoundWave> SoundObj3(TEXT("SoundWave'/Game/Rewind/BGM/FM/dragon_roar10'"));
    if (SoundObj3.Succeeded()) {
        EQSound = SoundObj3.Object;
    }



    isDeadEnd = false;
}


void ABoss::ChangeState(BMonsterAIState22 _eNextState, bool _bForce)
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

    if (!bIsDead) {
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
                    //canWalking = false;
                    AnimInstance->Montage_Play(MTMontage, 1.2f);
                }

                //SummonMeteor();
              //  GetWorldTimerManager().SetTimer(UnusedHandle, this, &ABoss::SummonMeteor, 1.0f, false);
            }

            if (Distance < 200) {
                if (b_eMState == BMonsterAIState22::Roaming) {
                    ChangeState(BMonsterAIState22::Idle);
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

                if (b_eMState != BMonsterAIState22::Roaming) {
                    ChangeState(BMonsterAIState22::Roaming);
                }
                FVector Direction = (PlayerPawn->GetActorLocation() - GetActorLocation()).GetSafeNormal();
                FVector NewLocation = GetActorLocation() + Direction * MovementSpeed * DeltaTime;
                SetActorLocation(NewLocation);
            }
        }

    }
    else {
        if (!isDeadEnd) {
            isDeadEnd = true;
            ChangeState(BMonsterAIState22::Dead1);
           
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
            for (int i = 0; i < 5; i++)
            {
                FVector SpawnLocation = PlayerPawn->GetActorLocation() + FVector(FMath::RandRange(-500, 500), FMath::RandRange(-500, 500), 1200);
                FRotator SpawnRotation = (PlayerPawn->GetActorLocation() - SpawnLocation).Rotation();
                FActorSpawnParameters SpawnParams;
                SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

                AMeteor* Meteor = GetWorld()->SpawnActor<AMeteor>(AMeteor::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
            }

            bCanSummonMeteor = false;
            GetWorldTimerManager().SetTimer(TimerHandle_MeteorAttack, this, &ABoss::ResetMeteorCooldown, 5.0f, false);
        }
    }
    UGameplayStatics::PlaySoundAtLocation(this, MTSound, GetActorLocation());

}

void ABoss::Earthquake()
{
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (PlayerPawn)
    {
        UGameplayStatics::ApplyDamage(PlayerPawn, 10.f, GetController(), this, UDamageType::StaticClass());

        UGameplayStatics::PlaySoundAtLocation(this, EQSound, GetActorLocation());

        AP_Character* Character = Cast<AP_Character>(PlayerPawn);
        if (Character) {
            Character->CSEarthQ();
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
    UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
    //UE_LOG(LogTemp, Warning, TEXT("HP: %f"), hp);


    if (hp <= 0) {

        bIsDead = true;

        UGameplayStatics::PlaySoundAtLocation(this, EQSound, GetActorLocation());
        //UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        //if (AnimInstance && DMontage)
        //{
        //    AnimInstance->Montage_Play(DMontage, 2.2f);

        //    GetCharacterMovement()->DisableMovement();
        //    AAIController* AIController = Cast<AAIController>(GetController());
        //    if (AIController != nullptr)
        //    {
        //        AIController->StopMovement();
        //    }
            //UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());

    }

}

