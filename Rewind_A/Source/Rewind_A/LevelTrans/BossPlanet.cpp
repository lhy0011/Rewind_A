// Fill out your copyright notice in the Description page of Project Settings.


#include "BossPlanet.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ABossPlanet::ABossPlanet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 구형 충돌체를 사용하여 행성 생성
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComponent;


}

FVector ABossPlanet::CalculateGravity(AActor* OtherActor)
{
    // 다른 물체와의 거리 계산
    FVector PlanetLocation = GetActorLocation();
    FVector OtherLocation = OtherActor->GetActorLocation();
    float Distance = FVector::Distance(PlanetLocation, OtherLocation);

    // 중력 범위 안에 있는 경우에만 중력 적용
    if (Distance <= GravityRange)
    {
        // 질량 계산
        float OtherMass = OtherActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();

        // 중력 벡터 계산
        FVector Direction = (PlanetLocation - OtherLocation).GetSafeNormal();
        float Magnitude = GravityConstant * Mass * OtherMass / (Distance * Distance);
        FVector GravityVector = Magnitude * Direction;

        return GravityVector;
    }

    // 중력 범위 밖인 경우 중력을 적용하지 않음
    return FVector::ZeroVector;

}

// Called when the game starts or when spawned
void ABossPlanet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABossPlanet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

