// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/GameplayStatics.h"
#include "Monster.h"

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

void AMonsterAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    Super::OnMoveCompleted(RequestID, Result);

    // If the move was successful
    if (Result.Code == EPathFollowingResult::Success)
    {
        AMonster* Monster = Cast<AMonster>(GetPawn());
        if (Monster)
        {
            Monster->ChangeState(EMonsterAIState::Idle); // Assuming this method changes the monster's state to Idle
        }
    }
}

void AMonsterAIController::OnPerceptionUpdated(TArray<AActor*> UpdatedActors)
{
    //bool bPlayerDetected = false;

    //for (AActor* Actor : UpdatedActors)
    //{
    //    ACharacter* PlayerPawn = Cast<ACharacter>(Actor);
    //    if (PlayerPawn && PlayerPawn->IsPlayerControlled())
    //    {
    //        bPlayerDetected = true;
    //        OnPlayerDetected(PlayerPawn);
    //        break;
    //    }
    //}

    //if (!bPlayerDetected)
    //{
    //    OnPlayerLost();
    //}
}

void AMonsterAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    //if (Actor && Stimulus.IsValid() && Stimulus.WasSuccessfullySensed())
    //{
    //    GetBlackboardComponent()->SetValueAsObject("TargetPlayer", Actor);
    //}
    //else
    //{
    //    GetBlackboardComponent()->ClearValue("TargetPlayer");
    //}

    ACharacter* PlayerCharacter = Cast<ACharacter>(Actor);

    if (PlayerCharacter)
    {
        if (Stimulus.WasSuccessfullySensed()) // 플레이어를 발견했을 때
        {
            OnPlayerDetected(PlayerCharacter);
        }
        else // 플레이어를 잃었을 때
        {
            OnPlayerLost();
        }
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


