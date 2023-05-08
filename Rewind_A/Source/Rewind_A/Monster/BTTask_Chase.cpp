// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Chase.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Monster.h"

UBTTask_Chase::UBTTask_Chase()
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UBTTask_Chase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (!PlayerCharacter) return EBTNodeResult::Failed;

    AIController->MoveToActor(PlayerCharacter);
    return EBTNodeResult::Succeeded;
}