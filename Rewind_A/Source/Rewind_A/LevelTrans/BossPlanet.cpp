// Fill out your copyright notice in the Description page of Project Settings.


#include "BossPlanet.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ABossPlanet::ABossPlanet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ���� �浹ü�� ����Ͽ� �༺ ����
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComponent;


}

FVector ABossPlanet::CalculateGravity(AActor* OtherActor)
{
    // �ٸ� ��ü���� �Ÿ� ���
    FVector PlanetLocation = GetActorLocation();
    FVector OtherLocation = OtherActor->GetActorLocation();
    float Distance = FVector::Distance(PlanetLocation, OtherLocation);

    // �߷� ���� �ȿ� �ִ� ��쿡�� �߷� ����
    if (Distance <= GravityRange)
    {
        // ���� ���
        float OtherMass = OtherActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();

        // �߷� ���� ���
        FVector Direction = (PlanetLocation - OtherLocation).GetSafeNormal();
        float Magnitude = GravityConstant * Mass * OtherMass / (Distance * Distance);
        FVector GravityVector = Magnitude * Direction;

        return GravityVector;
    }

    // �߷� ���� ���� ��� �߷��� �������� ����
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

