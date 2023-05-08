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
    }

    static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBP(TEXT("AnimBlueprint'/Game/Rewind/Character/FireGolem/aim/FGolemBP_C'"));
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
    if (ATMontage.Succeeded())
    {
        DeathMontage = ATMontage.Object;
    }

    CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
    CollisionComponent->InitCapsuleSize(42.0f, 96.0f);
    CollisionComponent->SetCollisionProfileName("Pawn");
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    RootComponent = CollisionComponent;


    MaxHealth = 100.0f;
    AttackDamage = 2;
}

void AFGolem::BeginPlay()
{
    Super::BeginPlay();

    CurrentHealth = MaxHealth;

}