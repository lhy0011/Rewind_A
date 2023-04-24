// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include <Components/PoseableMeshComponent.h>

#include "AfterIMG_R.generated.h"

UCLASS()
class REWIND_A_API AAfterIMG_R : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAfterIMG_R();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	UPoseableMeshComponent* m_PoseCopy;
	float						m_fLifeTime;

	UMaterial* m_DefaltMtrl;



public:
	void SetSkeletalMeshComponent(USkeletalMeshComponent* _SKMC);
	void SetLifeTime(float _Time)
	{
		m_fLifeTime = _Time;
		SetLifeSpan(m_fLifeTime);
	}


};
