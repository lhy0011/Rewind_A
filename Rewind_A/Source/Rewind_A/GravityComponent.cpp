// Fill out your copyright notice in the Description page of Project Settings.


#include "GravityComponent.h"
#include "Components/PrimitiveComponent.h"
#include "MassComponent.cpp"


// Sets default values for this component's properties
UGravityComponent::UGravityComponent() : GravityConstant(9.8f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGravityComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGravityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


FVector UGravityComponent::CalculateGravityForce(const FVector& OtherLocation, float OtherMass) const
{
	FVector Direction = OtherLocation - GetOwner()->GetActorLocation();
	float Distance = Direction.Size();

	if (Distance == 0.f)
	{
		return FVector::ZeroVector;
	}

	float ForceMagnitude = (GravityConstant * GetMass() * OtherMass) / (Distance * Distance);
	FVector Force = Direction.GetSafeNormal() * ForceMagnitude;

	return Force;
}
