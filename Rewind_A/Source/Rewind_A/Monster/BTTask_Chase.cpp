// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Chase.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Monster.h"
#include "GameFramework/CharacterMovementComponent.h"

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

    AMonster* Monster = Cast<AMonster>(AIController->GetPawn());
    if (Monster) {
        // 이동속도
        UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(Monster->GetMovementComponent());
        if (CharacterMovement)
        {
            CharacterMovement->MaxWalkSpeed = 300.0f;
        }
    }

    float AcceptableDistance = 100.0f;  // 1m 주변까지
    AIController->MoveToActor(PlayerCharacter, AcceptableDistance);
    return EBTNodeResult::Succeeded;
}