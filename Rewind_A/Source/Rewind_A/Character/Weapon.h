// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"


#include "NiagaraSystem.h"
#include "Niagara/Public/NiagaraComponent.h"

#include "Weapon.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class AP_Character;

UCLASS()
class REWIND_A_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeapon();

	UPROPERTY()
		AP_Character* OwningCharacter;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UBoxComponent* CollisionBox;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
		float AttackDamage;

	void EnableCollision() { WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); }
	void DisableCollision() { WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); }

	UNiagaraComponent* NiagaraComponent;

};
