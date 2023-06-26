// Fill out your copyright notice in the Description page of Project Settings.


#include "DSkeleton.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"

ADSkeleton::ADSkeleton()
{
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(TEXT("SkeletalMesh'/Game/Rewind/Character/DesertSkeleton/FBX/DesertSkeleton'"));
    if (SkeletalMesh.Succeeded())
    {
        GetMesh()->SetSkeletalMesh(SkeletalMesh.Object);
        GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -89.f));
        GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
        GetMesh()->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
    }

    static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBP(TEXT("AnimBlueprint'/Game/Rewind/Character/DesertSkeleton/aim/DSkeletonBP.DSkeletonBP_C'"));
    if (AnimBP.Succeeded())
    {
        GetMesh()->SetAnimInstanceClass(AnimBP.Class);
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATMontage(TEXT("AnimMontage'/Game/Rewind/Character/DesertSkeleton/aim/DSkeletonAttack'"));
    if (ATMontage.Succeeded())
    {
        AttackMontage = ATMontage.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> DMontage(TEXT("AnimMontage'/Game/Rewind/Character/DesertSkeleton/aim/DSkeletonDead'"));
    if (DMontage.Succeeded())
    {
        DeathMontage = DMontage.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> HMontage(TEXT("AnimMontage'/Game/Rewind/Character/DesertSkeleton/aim/DSkeletonHit'"));
    if (HMontage.Succeeded())
    {
        HitMontage = HMontage.Object;
    }


    CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
    CollisionComponent->InitCapsuleSize(100.0f, 100.0f);

    CollisionComponent->SetCollisionProfileName(TEXT("OverlapAll"));
    CollisionComponent->SetupAttachment(RootComponent);


    MaxHealth = 200.0f;
    CurrentHealth = 90.0f;

    AttackDamage = 1;

    GemActor = ADGem::StaticClass();



    // 타임컨트롤

        // 노인
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> OSkeletalMeshAsset(TEXT("SkeletalMesh'/Game/Rewind/Character/DesertSkeleton/FBX/DesertSkeletonOld'"));
    if (OSkeletalMeshAsset.Succeeded())
    {
        OldMesh = OSkeletalMeshAsset.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimBlueprintGeneratedClass> OAnimationAsset(TEXT("AnimBlueprint'/Game/Rewind/Character/DesertSkeleton/aim/Old/DSOldBP.DSOldBP_C'"));
    if (OAnimationAsset.Succeeded())
    {
        OldAnimation = OAnimationAsset.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> OATMontage(TEXT("AnimMontage'/Game/Rewind/Character/DesertSkeleton/aim/Old/DSOAttack'"));
    if (OATMontage.Succeeded())
    {
        OAttackMontage = OATMontage.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> ODMontage(TEXT("AnimMontage'/Game/Rewind/Character/DesertSkeleton/aim/Old/DSODeath'"));
    if (ODMontage.Succeeded())
    {
        ODeathMontage = ODMontage.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> OHMontage(TEXT("AnimMontage'/Game/Rewind/Character/DesertSkeleton/aim/Old/DSOHit'"));
    if (OHMontage.Succeeded())
    {
        OHitMontage = OHMontage.Object;
    }

}

void ADSkeleton::UpdateStats()
{
    if (Age < -30)
    {
        CurrentHealth = 180;
    }
    else if (Age >= -30 && Age < -15)
    {
        CurrentHealth = 160;
    }
    else if (Age >= -15 && Age < -5)
    {
        CurrentHealth = 140;
    }
    else if (Age == 0)
    {
        CurrentHealth = 120;
    }
    else if (Age > 0 && Age < 8)
    {
        CurrentHealth = 100;
    }
    else if (Age >= 8 && Age < 20)
    {
        setOld();
        CurrentHealth = 85;
    }
    else if (Age >= 20)
    {
        CurrentHealth = 70;
    }

    //CurrentHealth = FMath::Clamp(CurrentHealth - Age * 2.0f, 20.0f, 300.0f);

    float newScale = 1.0f - Age * 0.002f;
    GetMesh()->SetWorldScale3D(FVector(newScale));
}

void ADSkeleton::BeginPlay()
{
    Super::BeginPlay();
}
