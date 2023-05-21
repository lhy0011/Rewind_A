// Fill out your copyright notice in the Description page of Project Settings.


#include "ToFireInVolume.h"
#include "../Character/P_Character.h"
#include "../Character/ReGameInstance.h"
#include <Engine/Classes/Components/BoxComponent.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
AToFireInVolume::AToFireInVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TransferVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TransferVolume"));
	RootComponent = TransferVolume;
	TransferVolume->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

}

// Called when the game starts or when spawned
void AToFireInVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AToFireInVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AToFireInVolume::NotifyActorBeginOverlap(AActor* OtherActor)
{
	AP_Character* Character = Cast<AP_Character>(OtherActor);
	if (Character) {
		UE_LOG(LogTemp, Warning, TEXT("Overlap event triggered1"));
		// 아래 상태로 두면 언리얼 프로젝트 내에서 배치 후 디테일 패널에서 이름을 입력할 수 있음
		//UGameplayStatics::OpenLevel(GetWorld(), FName(TransferLevelName));
		UReGameInstance* GameInstance = Cast<UReGameInstance>(GetWorld()->GetGameInstance());
		if (GameInstance) {
			UE_LOG(LogTemp, Warning, TEXT("Overlap event triggered2"));
			GameInstance->setCharacterState(
				Character->getCPotion(),
				Character->getCHP(),
				Character->getRecallUse(),
				Character->getControlUse(),
				Character->getIsGotFG(),
				Character->getIsGotDG(),
				Character->getIsGotIG(),
				Character->getIsGotMG()
			);

			UGameplayStatics::OpenLevel(GetWorld(), FName(TransferLevelName));

		}
	}
}
