// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Boss.generated.h"

UCLASS()
class REWIND_A_API ABoss : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// 체력
	float hp;
	

	// 메테오
	UFUNCTION(BlueprintCallable, Category = "Abilities")
		void SummonMeteor();

	UFUNCTION(BlueprintCallable, Category = "Abilities")
		void Earthquake();

	FTimerHandle TimerHandle_EarthquakeAttack;

	FTimerHandle TimerHandle_EarthquakeAttackCooldown;

	bool bIsEarthquakeAttackOnCooldown = false;

	void EndEarthquakeCooldown();
};
