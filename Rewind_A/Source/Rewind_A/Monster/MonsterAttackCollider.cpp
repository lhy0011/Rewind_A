// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAttackCollider.h"
#include "Components/SphereComponent.h"
#include "Monster.h"
#include "../Character/P_Character.h"

// Sets default values
AMonsterAttackCollider::AMonsterAttackCollider()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
    RootComponent = CollisionComponent;
    CollisionComponent->InitSphereRadius(100.0f);
    CollisionComponent->SetCollisionProfileName(TEXT("OverlapAll"));

    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMonsterAttackCollider::OnOverlapBegin);

}

// Called when the game starts or when spawned
void AMonsterAttackCollider::BeginPlay()
{
	Super::BeginPlay();

    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called every frame
void AMonsterAttackCollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMonsterAttackCollider::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
    {
        AP_Character* character = Cast<AP_Character>(OtherActor);
        if (character != nullptr)
        {
            character->TakeDamage(OwningCharacter->AttackDamage);
            //CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }
    }
}

void AMonsterAttackCollider::EnableCollision()
{
    CollisionComponent->SetCollisionProfileName(TEXT("OverlapAll"));
}

void AMonsterAttackCollider::DisableCollision()
{
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

