// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "P_Character.h"
#include "../Monster/FGolem.h"
#include "../Monster/Monster.h"
#include "../Boss/Boss.h"
#include "../Item/TimeLockableActor.h"


#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.


	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("/Script/Engine.StaticMesh'/Game/Rewind/Character/Main_Character/FBX/Sword'"));

	if (mesh.Succeeded())
	{
		WeaponMesh->SetStaticMesh(mesh.Object);
	}


	WeaponMesh->SetCollisionProfileName(TEXT("OverlapAll"));
	WeaponMesh->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnOverlapBegin);

	PrimaryActorTick.bCanEverTick = true;

	AttackDamage = 17.0f;



	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("AttackParticle"));
	NiagaraComponent->SetupAttachment(WeaponMesh);

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>NiagaraDissolveAsset(TEXT("NiagaraSystem'/Game/Rewind/Practice/Main_Character_swordtrail'"));
	if (NiagaraDissolveAsset.Succeeded()) {
		NiagaraComponent->SetAsset(NiagaraDissolveAsset.Object);
		NiagaraComponent->bAutoActivate = false;
		NiagaraComponent->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f)); 
		NiagaraComponent->SetRelativeRotation(FRotator(-90.0f, 00.0f, -90.0f));
		NiagaraComponent->SetRelativeLocation(FVector(0.0f, -0.6f, 0.0f));
	}


}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	NiagaraComponent->SetActive(false);
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{

		if (OwningCharacter && OwningCharacter->isComboAttacking)
		{
			//OwningCharacter->CSAttack();
			AMonster* Mst = Cast<AMonster>(OtherActor);
			if (Mst) {
				if (Mst->bIsTimeLocked)
				{
					Mst->StoreDamage(AttackDamage);
				}
				else
				{
					Mst->TakeMonsterDamage(AttackDamage, OwningCharacter);
				}
			}

			// 타임록 물체
			if (OwningCharacter && OwningCharacter->isComboAttacking && OwningCharacter->isTimeLocking)
			{
				ATimeLockableActor* TLac = Cast<ATimeLockableActor>(OtherActor);
				if (TLac)
				{
					TLac->StoreDamage(AttackDamage);
					TLac->LastHitDirection = OwningCharacter->GetActorForwardVector();
				}
			}

			ABoss* bo = Cast<ABoss>(OtherActor);
			if (bo) {
				bo->TakeMonsterDamage(AttackDamage, OwningCharacter);
			}
		}
	}
}

