// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AMonsterAIController::AMonsterAIController()
{
    PrimaryActorTick.bCanEverTick = true;

    AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

    UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    SightConfig->SightRadius = 2000.0f;
    SightConfig->LoseSightRadius = 2200.0f;
    SightConfig->PeripheralVisionAngleDegrees = 120.0f;
    AIPerceptionComponent->ConfigureSense(*SightConfig);


    //static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeObject(TEXT("BehaviorTree'/Game/Rewind/Character/Monster/MonsterBT'"));
    //if (BehaviorTreeObject.Succeeded())
    //{
    //    MonsterBehaviorTree = BehaviorTreeObject.Object;
    //}


    static ConstructorHelpers::FObjectFinder<UObject> BehaviorTreeObject(TEXT("BehaviorTree'/Game/Rewind/Character/Monster/MonsterBT'"));
    if (BehaviorTreeObject.Succeeded())
    {
        MonsterBehaviorTree = Cast<UBehaviorTree>(BehaviorTreeObject.Object);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load MonsterBT"));
    }
}

void AMonsterAIController::BeginPlay()
{
    Super::BeginPlay();

    AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AMonsterAIController::OnTargetPerceptionUpdated);

    if (MonsterBehaviorTree)
    {
        RunBehaviorTree(MonsterBehaviorTree);
    }
}

void AMonsterAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMonsterAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    if (Actor && Stimulus.IsValid() && Stimulus.WasSuccessfullySensed())
    {
        GetBlackboardComponent()->SetValueAsObject("TargetPlayer", Actor);
    }
    else
    {
        GetBlackboardComponent()->ClearValue("TargetPlayer");
    }
}

void AMonsterAIController::OnPlayerDetected(ACharacter* PlayerPawn)
{
    if (PlayerPawn)
    {
        GetBlackboardComponent()->SetValueAsObject("TargetPlayer", Cast<UObject>(PlayerPawn));

        GetBlackboardComponent()->SetValueAsBool("IsDetected", true);
    }
}

void AMonsterAIController::OnPlayerLost()
{
    GetBlackboardComponent()->ClearValue("TargetPlayer");

    GetBlackboardComponent()->SetValueAsBool("IsPlayerDetected", false);
}


