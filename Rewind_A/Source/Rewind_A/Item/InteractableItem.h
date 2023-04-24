// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableItem.generated.h"

UCLASS()
class REWIND_A_API AInteractableItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableItem();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UBoxComponent* BoxCollider;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable, Category = "Item")
		void OnInteract(USceneComponent* AttachTo);



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		float HoldingDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		float VerticalOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		float HorizontalOffset;


	// æ∆øÙ∂Û¿Œ
	UFUNCTION(BlueprintCallable, Category = "Item")
		void EnableOutline();

	UFUNCTION(BlueprintCallable, Category = "Item")
		void DisableOutline();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
