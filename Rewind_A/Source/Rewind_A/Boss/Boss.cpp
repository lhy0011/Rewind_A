// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss.h"
#include "Kismet/GameplayStatics.h"
#include "Meteor.h"

// Sets default values
ABoss::ABoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	hp = 1000;
}

// Called when the game starts or when spawned
void ABoss::BeginPlay()
{
	Super::BeginPlay();
	
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (PlayerPawn)
    {
        float Distance = FVector::Distance(GetActorLocation(), PlayerPawn->GetActorLocation());
        if (Distance <= 500.f && !bIsEarthquakeAttackOnCooldown)
        {
            Earthquake();

            bIsEarthquakeAttackOnCooldown = true;
            GetWorld()->GetTimerManager().SetTimer(TimerHandle_EarthquakeAttackCooldown, this, &ABoss::EndEarthquakeCooldown, 60.0f, false);
        }
    }
}

// Called every frame
void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABoss::SummonMeteor()
{
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (PlayerPawn)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        FVector SpawnLocation = GetActorLocation() + FVector(0, 0, 500);
        FRotator SpawnRotation = (PlayerPawn->GetActorLocation() - SpawnLocation).Rotation();
        AMeteor* Meteor = GetWorld()->SpawnActor<AMeteor>(AMeteor::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);

    }
}

void ABoss::Earthquake()
{
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (PlayerPawn)
    {
        float Distance = FVector::Distance(GetActorLocation(), PlayerPawn->GetActorLocation());
        if (Distance <= 500.f)
        {
            UGameplayStatics::ApplyDamage(PlayerPawn, 10.f, GetController(), this, UDamageType::StaticClass());
        }
        else
        {
            GetWorld()->GetTimerManager().ClearTimer(TimerHandle_EarthquakeAttack);
        }
    }
}

void ABoss::EndEarthquakeCooldown()
{
	bIsEarthquakeAttackOnCooldown = false;
}

