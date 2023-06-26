// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Meteor.generated.h"

UCLASS()
class REWIND_A_API AMeteor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMeteor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere, Category = "Movement")
		float Speed;

protected:
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UStaticMeshComponent* MeteorMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class USphereComponent* CollisionSphere;

	AActor* Target;


	FVector InitialTargetLocation;

	FTimerHandle DestroyTimerHandle;
	void DestroyMeteor();


	UPROPERTY(EditAnywhere, Category = "Effects")
		UParticleSystem* ExplosionEffect;

	void PlayExplosionEffect();
};
