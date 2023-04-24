// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimeLock.generated.h"

UCLASS()
class REWIND_A_API ATimeLock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATimeLock();

	UFUNCTION()
		void Activate(UPrimitiveComponent* TargetComponent);

	UFUNCTION()
		void Deactivate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Time Lock")
		float Duration;

	UPROPERTY(EditDefaultsOnly, Category = "Time Lock")
		float DamagePerSecond;

	//UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	//	TSubclassOf<class AProjectile> ProjectileClass;

private:
	UPROPERTY()
		UPrimitiveComponent* LockedComponent;

	FVector OriginalLinearVelocity;
	FVector OriginalAngularVelocity;

	float AccumulatedDamage;

};
