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

    CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
    CollisionComponent->InitCapsuleSize(100.0f, 100.0f);

    CollisionComponent->SetCollisionProfileName(TEXT("OverlapAll"));
    CollisionComponent->SetupAttachment(RootComponent);



    MaxHealth = 200.0f;
    CurrentHealth = 90.0f;

    AttackDamage = 1;

    GemActor = ADGem::StaticClass();

}

void ADSkeleton::UpdateStats()
{
    if (Age < -30)
    {
        CurrentHealth = 160;
    }
    else if (Age >= -30 && Age < -15)
    {
        CurrentHealth = 140;
    }
    else if (Age >= -15 && Age < -5)
    {
        CurrentHealth = 120;
    }
    else if (Age == 0)
    {
        CurrentHealth = 100;
    }
    else if (Age > 0 && Age < 5)
    {
        CurrentHealth = 80;
    }
    else if (Age >= 5 && Age < 20)
    {
        CurrentHealth = 60;
    }
    else if (Age >= 20)
    {
        CurrentHealth = 40;
    }

    //CurrentHealth = FMath::Clamp(CurrentHealth - Age * 2.0f, 20.0f, 300.0f);

    float newScale = 1.0f - Age * 0.002f;
    GetMesh()->SetWorldScale3D(FVector(newScale));
}

void ADSkeleton::BeginPlay()
{
    Super::BeginPlay();
}
