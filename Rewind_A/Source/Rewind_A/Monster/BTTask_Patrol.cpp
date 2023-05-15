// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Patrol.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Monster.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_Patrol::UBTTask_Patrol()
{
    NodeName = TEXT("Move To Random Location");
}

EBTNodeResult::Type UBTTask_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    APawn* ControlledPawn = AIController->GetPawn();
    if (!ControlledPawn)
    {
        return EBTNodeResult::Failed;
    }

    FVector Origin = ControlledPawn->GetActorLocation();
    FNavLocation RandomLocation;

    UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(ControlledPawn->GetWorld());
    if (!NavSystem || !NavSystem->GetRandomReachablePointInRadius(Origin, 1000.0f, RandomLocation))
    {
        return EBTNodeResult::Failed;
    }


    AMonster* Monster = Cast<AMonster>(AIController->GetPawn());
    if (Monster)
    {
        // 이동속도
        UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(Monster->GetMovementComponent());
        if (CharacterMovement)
        {
            CharacterMovement->MaxWalkSpeed = 150.0f;
        }

        UE_LOG(LogTemp, Warning, TEXT("Roaming"));;
        Monster->ChangeState(EMonsterAIState::Roaming);
    }

    AIController->MoveToLocation(RandomLocation.Location, -1.0f);

    return EBTNodeResult::Succeeded;
}