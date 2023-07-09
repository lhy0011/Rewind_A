// Fill out your copyright notice in the Description page of Project Settings.


#include "GIceGolem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"

AGIceGolem::AGIceGolem()
{
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(TEXT("SkeletalMesh'/Game/Rewind/Character/Glacier/FBX/IceGolem'"));
    if (SkeletalMesh.Succeeded())
    {
        GetMesh()->SetSkeletalMesh(SkeletalMesh.Object);
        GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -89.f));
        GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
        GetMesh()->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
    }

    static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBP(TEXT("AnimBlueprint'/Game/Rewind/Character/Glacier/aim/GIceBP.GIceBP_C'"));
    if (AnimBP.Succeeded())
    {
        GetMesh()->SetAnimInstanceClass(AnimBP.Class);
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATMontage(TEXT("AnimMontage'/Game/Rewind/Character/Glacier/aim/IceAttack'"));
    if (ATMontage.Succeeded())
    {
        AttackMontage = ATMontage.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> DMontage(TEXT("AnimMontage'/Game/Rewind/Character/Glacier/aim/IceDeath'"));
    if (DMontage.Succeeded())
    {
        DeathMontage = DMontage.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> HMontage(TEXT("AnimMontage'/Game/Rewind/Character/Glacier/aim/IceHit'"));
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

    GemActor = AGGem::StaticClass();



    // 타임컨트롤

    // 어린이
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> CSkeletalMeshAsset(TEXT("SkeletalMesh'/Game/Rewind/Character/Glacier/FBX/iceGolemChild'"));
    if (CSkeletalMeshAsset.Succeeded())
    {
        ChildMesh = CSkeletalMeshAsset.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimBlueprintGeneratedClass> CAnimationAsset(TEXT("AnimBlueprint'/Game/Rewind/Character/Glacier/aim/Child/IGChildBP.IGChildBP_C'"));
    if (CAnimationAsset.Succeeded())
    {
        ChildAnimation = CAnimationAsset.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> CATMontage(TEXT("AnimMontage'/Game/Rewind/Character/Glacier/aim/Child/IGCAttack'"));
    if (CATMontage.Succeeded())
    {
        CAttackMontage = CATMontage.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> CDMontage(TEXT("AnimMontage'/Game/Rewind/Character/Glacier/aim/Child/IGCDeath'"));
    if (CDMontage.Succeeded())
    {
        CDeathMontage = CDMontage.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> CHMontage(TEXT("AnimMontage'/Game/Rewind/Character/Glacier/aim/Child/IGCHit'"));
    if (CHMontage.Succeeded())
    {
        CHitMontage = CHMontage.Object;
    }


    // 노인
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> OSkeletalMeshAsset(TEXT("SkeletalMesh'/Game/Rewind/Character/Glacier/FBX/iceGolemOld'"));
    if (OSkeletalMeshAsset.Succeeded())
    {
        OldMesh = OSkeletalMeshAsset.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimBlueprintGeneratedClass> OAnimationAsset(TEXT("AnimBlueprint'/Game/Rewind/Character/Glacier/aim/Old/IGOldBP.IGOldBP_C'"));
    if (OAnimationAsset.Succeeded())
    {
        OldAnimation = OAnimationAsset.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> OATMontage(TEXT("AnimMontage'/Game/Rewind/Character/Glacier/aim/Old/IGOAttack'"));
    if (OATMontage.Succeeded())
    {
        OAttackMontage = OATMontage.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> ODMontage(TEXT("AnimMontage'/Game/Rewind/Character/Glacier/aim/Old/IGODeath'"));
    if (ODMontage.Succeeded())
    {
        ODeathMontage = ODMontage.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> OHMontage(TEXT("AnimMontage'/Game/Rewind/Character/Glacier/aim/Old/IGOHit'"));
    if (OHMontage.Succeeded())
    {
        OHitMontage = OHMontage.Object;
    }


    // 아기
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> BSkeletalMeshAsset(TEXT("SkeletalMesh'/Game/Rewind/Character/Glacier/FBX/iceGolemBaby'"));
    if (BSkeletalMeshAsset.Succeeded())
    {
        BabyMesh = BSkeletalMeshAsset.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimBlueprintGeneratedClass> BAnimationAsset(TEXT("AnimBlueprint'/Game/Rewind/Character/Glacier/aim/Baby/IGBabyBP.IGBabyBP_C'"));
    if (BAnimationAsset.Succeeded())
    {
        BabyAnimation = BAnimationAsset.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> BATMontage(TEXT("AnimMontage'/Game/Rewind/Character/Glacier/aim/Baby/IGBAttack'"));
    if (BATMontage.Succeeded())
    {
        BAttackMontage = BATMontage.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> BDMontage(TEXT("AnimMontage'/Game/Rewind/Character/Glacier/aim/Baby/IGBDeath'"));
    if (BDMontage.Succeeded())
    {
        BDeathMontage = BDMontage.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> BHMontage(TEXT("AnimMontage'/Game/Rewind/Character/Glacier/aim/Baby/IGBHit'"));
    if (BHMontage.Succeeded())
    {
        BHitMontage = BHMontage.Object;
    }

}

void AGIceGolem::UpdateStats()
{
    if (Age < -30)
    {
        CurrentHealth = 160;
    }
    else if (Age >= -30 && Age < -15)
    {
        CurrentHealth = 140;
        setOld();
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

        setChild();
    }
    else if (Age >= 5 && Age < 15)
    {
        CurrentHealth = 60;
    }
    else if (Age >= 15)
    {
        CurrentHealth = 40;

        setBaby();
    }

    //CurrentHealth = FMath::Clamp(CurrentHealth - Age * 2.0f, 20.0f, 300.0f);

    float newScale = 1.0f - Age * 0.002f;
    GetMesh()->SetWorldScale3D(FVector(newScale));
}

void AGIceGolem::BeginPlay()
{
    Super::BeginPlay();


    Weapon->CollisionComponent->InitSphereRadius(200.0f);
}
