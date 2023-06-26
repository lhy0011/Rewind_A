// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAttackCollider.h"
#include "Components/SphereComponent.h"
#include "Boss.h"
#include "../Character/P_Character.h"

// Sets default values
ABossAttackCollider::ABossAttackCollider()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->InitSphereRadius(500.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAll"));

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABossAttackCollider::OnOverlapBegin);


}

// Called when the game starts or when spawned
void ABossAttackCollider::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called every frame
void ABossAttackCollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABossAttackCollider::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

	void ABossAttackCollider::EnableCollision()
{
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAll"));
}

void ABossAttackCollider::DisableCollision()
{
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

