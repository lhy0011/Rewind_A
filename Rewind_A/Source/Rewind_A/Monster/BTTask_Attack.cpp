// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "Monster.h"
#include "AIController.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = "Attack Player";
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    AMonster* Monster = Cast<AMonster>(AIController->GetPawn());
    if (!Monster || Monster->IsDead()) return EBTNodeResult::Failed;

   // Monster->OnAttack();

    return EBTNodeResult::Succeeded;
}
