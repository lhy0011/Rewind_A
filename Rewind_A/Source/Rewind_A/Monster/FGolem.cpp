// Fill out your copyright notice in the Description page of Project Settings.


#include "FGolem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"


AFGolem::AFGolem()
{
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(TEXT("SkeletalMesh'/Game/Rewind/Character/FireGolem/FBX/FireGolem'"));
    if (SkeletalMesh.Succeeded())
    {
        GetMesh()->SetSkeletalMesh(SkeletalMesh.Object);
        GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -89.f));
        GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
        GetMesh()->SetWorldScale3D(FVector(0.3f, 0.3f, 0.3f));

        GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    }



    static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBP(TEXT("AnimBlueprint'/Game/Rewind/Character/FireGolem/aim/FGolemBP.FGolemBP_C'"));
    if (AnimBP.Succeeded())
    {
        GetMesh()->SetAnimInstanceClass(AnimBP.Class);
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATMontage(TEXT("AnimMontage'/Game/Rewind/Character/FireGolem/aim/FGolemAttack'"));
    if (ATMontage.Succeeded())
    {
        AttackMontage = ATMontage.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> DMontage(TEXT("AnimMontage'/Game/Rewind/Character/FireGolem/aim/FGolemDead'"));
    if (DMontage.Succeeded())
    {
        DeathMontage = DMontage.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> HMontage(TEXT("AnimMontage'/Game/Rewind/Character/FireGolem/aim/FGolemHit'"));
    if (HMontage.Succeeded())
    {
        HitMontage = HMontage.Object;
    }
   

    CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
    CollisionComponent->InitCapsuleSize(150.0f, 100.0f);

    CollisionComponent->SetCollisionProfileName(TEXT("OverlapAll"));
    //CollisionComponent->SetCollisionObjectType(ECC_GameTraceChannel1);

    //CollisionComponent->SetCollisionProfileName(TEXT("OverlapAll"));
    CollisionComponent->SetupAttachment(RootComponent);

    //CollisionComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);

    MaxHealth = 200.0f;
    CurrentHealth = 100.0f;

    AttackDamage = 1;

    GemActor = AFGem::StaticClass();
}

void AFGolem::UpdateStats()
{
    if (Age > 30)
    {
        CurrentHealth = 250;
    }
    else if (Age <= 30 && Age > 15)
    {
        CurrentHealth = 200;
    }
    else if (Age <= 15 && Age > 5)
    {
        CurrentHealth = 150;
    }
    else if (Age == 0)
    {
        CurrentHealth = 100;
    }
    else if (Age<0 && Age>-5)
    {
        CurrentHealth = 80;
    }
    else if (Age <= -5 && Age > -20)
    {
        CurrentHealth = 60;
    }
    else if (Age <= -20)
    {
        CurrentHealth = 40;
    }

    //CurrentHealth = FMath::Clamp(CurrentHealth - Age * 2.0f, 20.0f, 300.0f);

    float newScale = 0.3f + Age * 0.005f;
    GetMesh()->SetWorldScale3D(FVector(newScale));
}

void AFGolem::BeginPlay()
{
    Super::BeginPlay();

    CurrentHealth = MaxHealth;
    Weapon->CollisionComponent->InitSphereRadius(200.0f);
}
