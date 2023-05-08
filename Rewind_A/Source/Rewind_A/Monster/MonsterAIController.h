// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "MonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class REWIND_A_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()
	
public:
    AMonsterAIController();

    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

    UPROPERTY()
        class UBehaviorTree* MonsterBehaviorTree;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
        class UAIPerceptionComponent* AIPerceptionComponent;

    UFUNCTION()
        void OnPerceptionUpdated(TArray<AActor*> UpdatedActors);

    UFUNCTION()
        void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

    UFUNCTION(BlueprintCallable)
        void OnPlayerDetected(ACharacter* PlayerPawn);

    UFUNCTION(BlueprintCallable)
        void OnPlayerLost();


};
