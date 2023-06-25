// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss.h"
#include "Kismet/GameplayStatics.h"
#include "Meteor.h"

// Sets default values
ABoss::ABoss()
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

    static ConstructorHelpers::FClassFinder<UAnimInstance> BossAnim(TEXT("AnimBlueprint'/Game/Rewind/Character/FireBoss/aim/BossAnim.BossAnim_C'"));
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



    bIsEarthquakeAttackOnCooldown = false;
    bCanSummonMeteor = false;

    MovementSpeed = 100.f;

    MeteorAttackThreshold = 1000.f;
    EarthquakeAttackThreshold = 500.f;
    MeleeAttackThreshold = 200.f;
}


// Called when the game starts or when spawned
void ABoss::BeginPlay()
{
    Super::BeginPlay();

    GetWorldTimerManager().SetTimer(TimerHandle_MeteorAttack, this, &ABoss::ResetMeteorCooldown, 5.0f, false);

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
        if (Distance <= MeleeAttackThreshold)
        {
            UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
            if (AnimInstance && ATMontage)
            {
                AnimInstance->Montage_Play(ATMontage, 0.8f);
            }
        }

        // 지진공격
        else if (Distance <= EarthquakeAttackThreshold && !bIsEarthquakeAttackOnCooldown)
        {

            UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
            if (AnimInstance && EQMontage)
            {
                AnimInstance->Montage_Play(EQMontage, 0.8f);
            }

            // 지진
            Earthquake();

            // 쿨타임
            bIsEarthquakeAttackOnCooldown = true;
            GetWorld()->GetTimerManager().SetTimer(TimerHandle_EarthquakeAttackCooldown, this, &ABoss::EndEarthquakeCooldown, 60.0f, false);
        }

        // 메테오
        else if (Distance > MeteorAttackThreshold)
        {
            UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
            if (AnimInstance && EQMontage)
            {
                AnimInstance->Montage_Play(MTMontage, 0.8f);
            }
            SummonMeteor();

        }

        else // 플레이어에게 이동
        {
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
        float Distance = FVector::Distance(GetActorLocation(), PlayerPawn->GetActorLocation());
        if (Distance <= 500.f)
        {
            UGameplayStatics::ApplyDamage(PlayerPawn, 10.f, GetController(), this, UDamageType::StaticClass());
        }
        else
        {
            GetWorld()->GetTimerManager().ClearTimer(TimerHandle_EarthquakeAttack);
        }
    }
}

void ABoss::EndEarthquakeCooldown()
{
    bIsEarthquakeAttackOnCooldown = false;
}

void ABoss::ResetMeteorCooldown()
{
    UE_LOG(LogTemp, Warning, TEXT("Reset mt"));
    bCanSummonMeteor = true;
}