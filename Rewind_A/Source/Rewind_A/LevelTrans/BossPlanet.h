// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossPlanet.generated.h"

UCLASS()
class REWIND_A_API ABossPlanet : public AActor : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossPlanet();

    // ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        float Mass;

    // �߷� ���
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        float GravityConstant;

    // �߷� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        float GravityRange;

public:
    // �߷��� ����ϴ� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Gravity")
        FVector CalculateGravity(AActor* OtherActor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
