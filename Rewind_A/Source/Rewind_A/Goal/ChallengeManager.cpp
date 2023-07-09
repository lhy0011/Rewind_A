

#include "ChallengeManager.h"
#include "MoveToGlacier.h"
#include "KillGlacier.h"

    UChallengeManager::UChallengeManager(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
        InitializeChallenges();
    }

    void UChallengeManager::InitializeChallenges()
    {
        Challenges.Add(NewObject<UMoveToGlacier>());
        Challenges.Add(NewObject<UKillGlacier>());

        if (Challenges.Num() > 0)
        {
            CurrentChallenge = Challenges[0];
        }
    }

    void UChallengeManager::CheckCurrentChallenge()
    {
        if (CurrentChallenge && CurrentChallenge->IsChallengeCompleted())
        {
            Challenges.RemoveAt(0);

            if (Challenges.Num() > 0)
            {
                CurrentChallenge = Challenges[0];
            }
            else
            {
                CurrentChallenge = nullptr;
            }
        }
    }

    //void UChallengeManager::UpdateChallenges()
    //{
    //    for (int i = 0; i < Challenges.Num(); i++)
    //    {
    //        if (Challenges[i]->IsChallengeCompleted())
    //        {
    //            Challenges.RemoveAt(i);
    //            i--;
    //        }
    //    }

    //    if (!CurrentChallenge && Challenges.Num() > 0)
    //    {
    //        CurrentChallenge = Challenges[0];
    //    }
    //}

    FString UChallengeManager::GetCurrentChallengeDescription()
    {

        if (CurrentChallenge)
        {
            return CurrentChallenge->GetChallengeDescription();
        }

        return FString("Clear");
    }
