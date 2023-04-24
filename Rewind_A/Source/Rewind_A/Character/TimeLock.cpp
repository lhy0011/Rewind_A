// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeLock.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/DamageEvents.h"

// Sets default values
ATimeLock::ATimeLock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    Duration = 5.0f;
    DamagePerSecond = 10.0f;
    AccumulatedDamage = 0.0f;

}

// Called when the game starts or when spawned
void ATimeLock::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATimeLock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (LockedComponent)
    {
        AccumulatedDamage += DamagePerSecond * DeltaTime;
    }

}

void ATimeLock::Activate(UPrimitiveComponent* TargetComponent)
{
    if (TargetComponent && TargetComponent->IsSimulatingPhysics())
    {
        LockedComponent = TargetComponent;
        OriginalLinearVelocity = LockedComponent->GetPhysicsLinearVelocity();
        OriginalAngularVelocity = LockedComponent->GetPhysicsAngularVelocityInDegrees();
        LockedComponent->SetPhysicsLinearVelocity(FVector::ZeroVector);
        LockedComponent->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
    }
}

void ATimeLock::Deactivate()
{
    if (LockedComponent)
    {
        LockedComponent->SetPhysicsLinearVelocity(OriginalLinearVelocity);
        LockedComponent->SetPhysicsAngularVelocityInDegrees(OriginalAngularVelocity);

        AActor* LockedActor = LockedComponent->GetOwner();
        if (LockedActor)
        {
            FVector SpawnLocation = LockedActor->GetActorLocation();
            FRotator SpawnRotation = GetInstigator()->GetActorRotation();

            //AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
            //if (Projectile)
            //{
            //    FVector ForceDirection = GetInstigator()->GetActorForwardVector();
            //    float ForceMultiplier = 100.0f;
            //    FVector ForceToApply = ForceDirection * AccumulatedDamage * ForceMultiplier;
            //    Projectile->MeshComponent->AddForce(ForceToApply, NAME_None, true);
            //}
        }

        AccumulatedDamage = 0.0f;
    }
}
