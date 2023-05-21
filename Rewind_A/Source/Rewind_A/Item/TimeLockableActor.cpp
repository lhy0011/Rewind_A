// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeLockableActor.h"

// Sets default values
ATimeLockableActor::ATimeLockableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AccumulatedDamage = 0.0f;
	bIsTimeLocked = false;


	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;


	StaticMesh->SetSimulatePhysics(true); // 중력적용

	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	if (MeshAsset.Succeeded())
	{
		StaticMesh->SetStaticMesh(MeshAsset.Object);
	}

	UBoxComponent* BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ATimeLockableActor::OnOverlapBegin);
	BoxComponent->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));
	BoxComponent->SetCollisionProfileName(TEXT("OverlapAll"));
	BoxComponent->SetupAttachment(RootComponent);


	bOriginalSimulatePhysics = true;
	StartTimeLockTime = 0.0f;

	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));

}

// Called when the game starts or when spawned
void ATimeLockableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATimeLockableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATimeLockableActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ATimeLockableActor::StoreDamage(float Damage)
{
	// 곱하는 값 테스트해서 설정
	AccumulatedDamage += Damage * 100.0f;
	UE_LOG(LogTemp, Warning, TEXT("StoreL %f"), AccumulatedDamage);
}


void ATimeLockableActor::ApplyAccumulatedDamage()
{
	FVector Force = LastHitDirection * AccumulatedDamage * 1000.0f;
	UE_LOG(LogTemp, Warning, TEXT("LastHitDirection X: %f, Y: %f, Z: %f"), LastHitDirection.X, LastHitDirection.Y, LastHitDirection.Z);
	Force.Z = 200.0f; // 위쪽으로 조정

	if (StaticMesh)
	{
		StaticMesh->SetSimulatePhysics(true);
		StaticMesh->AddForce(Force);
	}

	// 데미지 리셋
	AccumulatedDamage = 0.0f;
}


void ATimeLockableActor::StartTimeLock()
{
	bOriginalSimulatePhysics = StaticMesh->GetBodyInstance()->bSimulatePhysics;

	StaticMesh->SetSimulatePhysics(false);

	StartTimeLockTime = GetWorld()->GetTimeSeconds();
}

