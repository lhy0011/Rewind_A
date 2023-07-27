// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/AudioComponent.h"
#include "Sound/SoundWave.h"

#include "Meteor.generated.h"

class AP_Character;

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

	UFUNCTION(BlueprintCallable)
	void DestroyMeteor();


	UPROPERTY(EditAnywhere, Category = "Effects")
		UParticleSystem* ExplosionEffect;

	UFUNCTION(BlueprintCallable)
	void PlayExplosionEffect();


	UFUNCTION(BlueprintCallable)
		void Explosion();


	AP_Character* PCharacter;




	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* ExplosionSound;

};
