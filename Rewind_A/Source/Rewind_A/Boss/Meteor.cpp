// Fill out your copyright notice in the Description page of Project Settings.


#include "Meteor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
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
    CollisionSphere->SetSphereRadius(8.0f);

    CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AMeteor::OnOverlapBegin);

    Speed = 4500.0f;


    MeteorMesh->SetWorldScale3D(FVector(12.0f, 12.0f, 12.0f));



    static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemAsset(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion'"));
    if (ParticleSystemAsset.Succeeded())
    {
        ExplosionEffect = ParticleSystemAsset.Object;
    }
}

// Called when the game starts or when spawned
void AMeteor::BeginPlay()
{
	Super::BeginPlay();
	
    Target = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    if (Target)
    {
        InitialTargetLocation = Target->GetActorLocation();
    }

    GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AMeteor::DestroyMeteor, 0.8f, false);

}

// Called every frame
void AMeteor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (Target)
    {
        FVector MoveDirection = (InitialTargetLocation - GetActorLocation()).GetSafeNormal();
        FVector NewLocation = GetActorLocation() + MoveDirection * Speed * DeltaTime;
        SetActorLocation(NewLocation);
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
        // Check if the overlapped actor is a StaticMeshActor
        else if (OtherComp->IsA(UStaticMeshComponent::StaticClass()))
        {
            PlayExplosionEffect();
            Destroy();
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

