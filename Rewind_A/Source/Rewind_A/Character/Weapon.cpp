// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "P_Character.h"

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

	// 콜리전
	//CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	//CollisionBox->SetupAttachment(WeaponMesh);
	//CollisionBox->SetBoxExtent(FVector(0.025f, 0.09f, 0.012f));
	//CollisionBox->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
	//WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WeaponMesh->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnOverlapBegin);

	PrimaryActorTick.bCanEverTick = true;

	AttackDamage = 20.0f;

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr && OtherActor->IsA(AP_Character::StaticClass())) {
		AP_Character* APC = Cast<AP_Character>(GetOwner());
		if (APC) {
			// 데미지 구현
		}
	}
}