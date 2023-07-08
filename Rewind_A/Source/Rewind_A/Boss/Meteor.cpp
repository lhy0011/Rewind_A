// Fill out your copyright notice in the Description page of Project Settings.


#include "Meteor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Boss.h"
#include "../Character/P_Character.h"
#include "Components/SphereComponent.h"

// Sets default values
AMeteor::AMeteor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    MeteorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeteorMesh"));
    RootComponent = MeteorMesh;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/Rewind/Maps/Fire/FBX/Boss/FireBossMap_Low'"));
    if (MeshAsset.Succeeded())
    {
        MeteorMesh->SetStaticMesh(MeshAsset.Object);
    }

    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    CollisionSphere->SetupAttachment(MeteorMesh);
    CollisionSphere->SetSphereRadius(6.0f);

    CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AMeteor::OnOverlapBegin);

    Speed = 4000.0f;

    MeteorMesh->SetWorldScale3D(FVector(12.0f, 12.0f, 12.0f));

    MeteorMesh->SetSimulatePhysics(false);
    MeteorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);


    static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemAsset(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion'"));
    if (ParticleSystemAsset.Succeeded())
    {
        ExplosionEffect = ParticleSystemAsset.Object;
    }

    PCharacter = Cast<AP_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

}

// Called when the game starts or when spawned
void AMeteor::BeginPlay()
{
	Super::BeginPlay();


    Target = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    if (Target)
    {
        FVector RandomOffset = FMath::VRand() * 2000.0f;
        RandomOffset.Z = -1600.0f;
        InitialTargetLocation = Target->GetActorLocation() + RandomOffset;
    }

    //GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AMeteor::DestroyMeteor, 0.7f, false);

}

// Called every frame
void AMeteor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (Target)
    {
        FVector MoveDirection = (InitialTargetLocation - GetActorLocation()).GetSafeNormal();
        FVector NewLocation = GetActorLocation() + MoveDirection * Speed * DeltaTime;
        SetActorLocation(NewLocation, false);
    }
}

void AMeteor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        if (OtherActor->IsA(AP_Character::StaticClass()))
        {
            AP_Character* Character = Cast<AP_Character>(OtherActor);
            if (Character)
            {
                //캐릭터 데미지 로직
                UGameplayStatics::ApplyDamage(Character, 2.0f, GetInstigatorController(), this, UDamageType::StaticClass());

                Character->TakeDamage(2);
                PlayExplosionEffect();
                Destroy();
            }
        }
    }
}

void AMeteor::DestroyMeteor()
{
    PlayExplosionEffect();
    Destroy();
}

void AMeteor::PlayExplosionEffect()
{
    if (ExplosionEffect)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), FRotator::ZeroRotator);
    }
}

void AMeteor::Explosion()
{
    if (FVector::Distance(GetActorLocation(), PCharacter->GetActorLocation()) <= 300.0f)
    {
        // Deal damage to the character.
        UGameplayStatics::ApplyDamage(PCharacter, 2.0f, GetInstigatorController(), this, UDamageType::StaticClass());

        PCharacter->TakeDamage(2);

    }

    PlayExplosionEffect();
    Destroy();
}

