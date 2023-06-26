// Fill out your copyright notice in the Description page of Project Settings.


#include "FGolem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"


AFGolem::AFGolem()
{

    HSize = 0.3f;
    CSize = 0.55f;
    OSize = 0.4f;
    BSize = 0.82f;


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







    // 타임컨트롤

    // 어린이
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> CSkeletalMeshAsset(TEXT("SkeletalMesh'/Game/Rewind/Character/FireGolem/FBX/FireGolemChild'"));
    if (CSkeletalMeshAsset.Succeeded())
    {
        ChildMesh = CSkeletalMeshAsset.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimBlueprintGeneratedClass> CAnimationAsset(TEXT("AnimBlueprint'/Game/Rewind/Character/FireGolem/aim/Child/FGolemCBP.FGolemCBP_C'"));
    if (CAnimationAsset.Succeeded())
    {
        ChildAnimation = CAnimationAsset.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> CATMontage(TEXT("AnimMontage'/Game/Rewind/Character/FireGolem/aim/Child/FGolemCAttack'"));
    if (CATMontage.Succeeded())
    {
        CAttackMontage = CATMontage.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> CDMontage(TEXT("AnimMontage'/Game/Rewind/Character/FireGolem/aim/Child/FGolemCDeath'"));
    if (CDMontage.Succeeded())
    {
        CDeathMontage = CDMontage.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> CHMontage(TEXT("AnimMontage'/Game/Rewind/Character/FireGolem/aim/Child/FGolemCHit'"));
    if (CHMontage.Succeeded())
    {
        CHitMontage = CHMontage.Object;
    }


    // 노인
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> OSkeletalMeshAsset(TEXT("SkeletalMesh'/Game/Rewind/Character/FireGolem/FBX/FireGolemOld'"));
    if (OSkeletalMeshAsset.Succeeded())
    {
        OldMesh = OSkeletalMeshAsset.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimBlueprintGeneratedClass> OAnimationAsset(TEXT("AnimBlueprint'/Game/Rewind/Character/FireGolem/aim/Old/FGolemOBP.FGolemOBP_C'"));
    if (OAnimationAsset.Succeeded())
    {
        OldAnimation = OAnimationAsset.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> OATMontage(TEXT("AnimMontage'/Game/Rewind/Character/FireGolem/aim/Old/OATMontage'"));
    if (OATMontage.Succeeded())
    {
        OAttackMontage = OATMontage.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> ODMontage(TEXT("AnimMontage'/Game/Rewind/Character/FireGolem/aim/Old/OFGolemDeath'"));
    if (ODMontage.Succeeded())
    {
        ODeathMontage = ODMontage.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> OHMontage(TEXT("AnimMontage'/Game/Rewind/Character/FireGolem/aim/Old/OFGolemHit'"));
    if (OHMontage.Succeeded())
    {
        OHitMontage = OHMontage.Object;
    }


    // 아기
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> BSkeletalMeshAsset(TEXT("SkeletalMesh'/Game/Rewind/Character/FireGolem/FBX/FireGolemBaby'"));
    if (BSkeletalMeshAsset.Succeeded())
    {
        BabyMesh = BSkeletalMeshAsset.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimBlueprintGeneratedClass> BAnimationAsset(TEXT("AnimBlueprint'/Game/Rewind/Character/FireGolem/aim/Baby/FGolemBBP.FGolemBBP_C'"));
    if (BAnimationAsset.Succeeded())
    {
        BabyAnimation = BAnimationAsset.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> BATMontage(TEXT("AnimMontage'/Game/Rewind/Character/FireGolem/aim/Baby/BFGAttack'"));
    if (BATMontage.Succeeded())
    {
        BAttackMontage = BATMontage.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> BDMontage(TEXT("AnimMontage'/Game/Rewind/Character/FireGolem/aim/Baby/BFGDeath'"));
    if (BDMontage.Succeeded())
    {
        BDeathMontage = BDMontage.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> BHMontage(TEXT("AnimMontage'/Game/Rewind/Character/FireGolem/aim/Baby/BFGHit'"));
    if (BHMontage.Succeeded())
    {
        BHitMontage = BHMontage.Object;
    }

}

void AFGolem::UpdateStats()
{
    if (Age > 30)
    {
        CurrentHealth = 250;
    }
    else if (Age <= 30 && Age > 15)
    {
        setOld();
        HSize = OSize;
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
    else if (Age<0 && Age>-7)
    {
        CurrentHealth = 80;
    }
    else if (Age <= -7 && Age > -20)
    {
        CurrentHealth = 60;
        HSize = CSize;
        setChild();
    }
    else if (Age <= -20)
    {
        setBaby();
        HSize = BSize;
        CurrentHealth = 40;
    }

    //CurrentHealth = FMath::Clamp(CurrentHealth - Age * 2.0f, 20.0f, 300.0f);

    float newScale = HSize + Age * 0.005f;
    GetMesh()->SetWorldScale3D(FVector(newScale));
}

void AFGolem::BeginPlay()
{
    Super::BeginPlay();

    CurrentHealth = MaxHealth;
    Weapon->CollisionComponent->InitSphereRadius(200.0f);
}
