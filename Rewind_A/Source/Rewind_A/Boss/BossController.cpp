// Fill out your copyright notice in the Description page of Project Settings.


#include "BossController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ABossController::BeginPlay()
{
    if (BossMonsterBehaviorTree)
    {
        RunBehaviorTree(BossMonsterBehaviorTree);

        APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        if (PlayerPawn)
        {
            GetBlackboardComponent()->SetValueAsObject(PlayerKey, PlayerPawn);
        }
    }
}

void ABossController::Tick(float DeltaSeconds)
{
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (PlayerPawn)
    {
        float Distance = FVector::Dist(PlayerPawn->GetActorLocation(), GetPawn()->GetActorLocation());
        if (Distance > 1000.0f) 
        {
            // Summon Meteor here
            // This is a placeholder, replace it with actual summoning code
            UE_LOG(LogTemp, Warning, TEXT("Summoning Meteor!"));
        }
    }
}
