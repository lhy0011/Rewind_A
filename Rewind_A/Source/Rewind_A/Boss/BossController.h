// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BossController.generated.h"

/**
 * 
 */
UCLASS()
class REWIND_A_API ABossController : public AAIController
{
	GENERATED_BODY()
	

public:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "AI")
        FName PlayerKey;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
        UBehaviorTree* BossMonsterBehaviorTree;
};
