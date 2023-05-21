// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../Character/Weapon.h"
#include "Components/BoxComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

#include "TimeLockableActor.generated.h"

UCLASS()
class REWIND_A_API ATimeLockableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATimeLockableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time Lock")
		float AccumulatedDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time Lock")
		bool bIsTimeLocked;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time Lock")
		FVector LastHitDirection;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 데미지 누적
	void StoreDamage(float Damage);

	void ApplyAccumulatedDamage();


	// Physics 조정
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UPhysicsHandleComponent* PhysicsHandle;

	void StartTimeLock();



	UPROPERTY()
		bool bOriginalSimulatePhysics;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* StaticMesh;

	UPROPERTY()
		float StartTimeLockTime;



	UMaterialInterface* m_OriginalMtrl; // 원래거

	UMaterialInterface* m_TimeLockMtrl; // 타임록 적용
};
