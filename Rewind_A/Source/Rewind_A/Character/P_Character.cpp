// Fill out your copyright notice in the Description page of Project Settings.


#include "P_Character.h"

#include "P_AnimInst.h"
#include "AfterIMG.h"
#include "AfterIMG_R.h"
#include "Weapon.h"
#include "../Item/InteractableItem.h"
#include "../Item/HealingPotion.h"
#include "ReGameInstance.h"

#include "Engine/World.h"
#include "Engine/Level.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"

int CCC = 0;

// Sets default values
AP_Character::AP_Character()
    : m_eState(EPLAYER_STATE::IDLE)
    //, m_AnimInst(nullptr)
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SetAsset();
    SetCamera();
    SetCollision();

    //cooldown
    RecallUse = 1.f;
    ControlUse = 1.f;
    tp1, tp2, tp3, tp4 = 0.f;


    // 상호작용 변수
    InteractionDistance = 200.f;
    CurrentInteractableItem = nullptr;

    CHP = 10;

    // 콤보 변수
    isComboAttacking = false;
    isComboAttackDown = false;
    isComboAttackNext = false;
    ComboAttackCount = 0;

    isJumping = false;

    fGemisGotten = false;
    dGemisGotten = false;
    iGemisGotten = false;
    mGemisGotten = false;


    isTimeControlling = false;
    isTimeLocking = false;
    AgeValue = 0.f;
}



// 플레이어 상태 변환 함수
void AP_Character::ChangeState(EPLAYER_STATE _eNextState, bool _bForce)
{

    if (m_eState == _eNextState) {
        return;
    }


    m_eState = _eNextState;

}

void AP_Character::TakeDamage(float DamageAmount)
{
    CHP -= DamageAmount;
    if (CHP <= 0.0f)
    {
        // 캐릭터가 죽는 로직
    }
}


// Called when the game starts or when spawned
void AP_Character::BeginPlay()
{
    Super::BeginPlay();

    m_AnimInst = Cast<UP_AnimInst>(GetMesh()->GetAnimInstance());

    // 무기 장착
    Weapon = GetWorld()->SpawnActor<AWeapon>(AWeapon::StaticClass());

    if (Weapon) {
        FName SocketName = TEXT("Sword");
        Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);

        FVector RelativeLocation = FVector(0.0f, 0.0f, 0.0f);
        FRotator RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
        FVector RelativeScale = FVector(1.0f, 1.0f, 1.0f);
        //// 세부 조정


        FTransform RelativeTransform = FTransform(RelativeRotation, RelativeLocation, RelativeScale);
        Weapon->WeaponMesh->SetRelativeTransform(RelativeTransform);

        Weapon->OwningCharacter = this;

    }


    UReGameInstance* GameInstance = Cast<UReGameInstance>(GetGameInstance());
    if (GameInstance)
    {
        if (!GameInstance->isFirstLoad)
        {
            setCharacterState(
                GameInstance->PotionCount,
                GameInstance->CHP,
                GameInstance->RecallUse,
                GameInstance->ControlUse,

                GameInstance->fGemisGotten,
                GameInstance->dGemisGotten,
                GameInstance->iGemisGotten,
                GameInstance->mGemisGotten
            );
        }
        else
        {
            GameInstance->isFirstLoad = false;
        }
    }


    GetWorld()->GetTimerManager().SetTimer(MyTimerHandle, this, &AP_Character::RecallCountNsave, 1.f, true, 1.f);

}


// Called every frame
void AP_Character::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // DeltaTime 얻기
    GetWorld()->GetDeltaSeconds();

    if (0.f == GetCharacterMovement()->Velocity.Size()) // 이동중이 아닌지 판단
    {
        ChangeState(EPLAYER_STATE::IDLE); // idle 상태로 전환
    }

    if (GetCharacterMovement()->IsFalling()) {
        isJumping = true;
    }
    else {
        isJumping = false;
    }


    TArray<AActor*> FoundMonsters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMonster::StaticClass(), FoundMonsters);

    float ClosestMonsterDistance = FLT_MAX;
    for (AActor* Monster : FoundMonsters)
    {
        float Distance = (Monster->GetActorLocation() - GetActorLocation()).Size();
        if (Distance < ClosestMonsterDistance)
        {
            ClosestMonsterDistance = Distance;
        }
    }

    // 충돌 감지시 카메라 뒤로 빼기
    if (ClosestMonsterDistance < DistMonster)
    {
        // 몬스터가 가까이 있을 때
        m_pSpringArm->TargetArmLength = FMath::FInterpTo(m_pSpringArm->TargetArmLength, 330.f, DeltaTime, 5.f);
    }
    else if (m_pSpringArm->IsCollisionFixApplied())
    {
        // 일반적으로 가까울 때
        m_pSpringArm->TargetArmLength = FMath::FInterpTo(m_pSpringArm->TargetArmLength, 200.f, DeltaTime, 5.f);
    }
    else
    {
        // 원위치
        m_pSpringArm->TargetArmLength = FMath::FInterpTo(m_pSpringArm->TargetArmLength, 270.f, DeltaTime, 5.f);
    }


    //주변 몬스터 탐색
    if (isTimeControlling)
    {
        UpdateNearestMonster();
    }
}


// Called to bind functionality to input
void AP_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AP_Character::CharacterMoveRight);
    PlayerInputComponent->BindAxis(TEXT("MoveFront"), this, &AP_Character::CharacterMoveFront);
    PlayerInputComponent->BindAxis(TEXT("RotationZ"), this, &AP_Character::CharacterRotationZ);
    PlayerInputComponent->BindAxis(TEXT("RotationY"), this, &AP_Character::CharacterRotationY);
    //PlayerInputComponent->BindAxis(TEXT("TimeControlWheelUp"), this, &AP_Character::OnMouseWheelScroll);
    //PlayerInputComponent->BindAxis(TEXT("TimeControlWheelDown"), this, &AP_Character::OnMouseWheelScrollDown);

    PlayerInputComponent->BindAction(TEXT("PAttack"), EInputEvent::IE_Pressed, this, &AP_Character::ComboAttackDown);
    PlayerInputComponent->BindAction(TEXT("PAttack"), EInputEvent::IE_Released, this, &AP_Character::ComboAttackUp);
    PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AP_Character::CharacterJump);
    PlayerInputComponent->BindAction(TEXT("SaveLocation"), EInputEvent::IE_Pressed, this, &AP_Character::CharacterSaveLocation);
    PlayerInputComponent->BindAction(TEXT("TPLocation1"), EInputEvent::IE_Pressed, this, &AP_Character::CharacterTPL1);
    PlayerInputComponent->BindAction(TEXT("TPLocation2"), EInputEvent::IE_Pressed, this, &AP_Character::CharacterTPL2);
    PlayerInputComponent->BindAction(TEXT("TPLocation3"), EInputEvent::IE_Pressed, this, &AP_Character::CharacterTPL3);
    PlayerInputComponent->BindAction(TEXT("TPLocation4"), EInputEvent::IE_Pressed, this, &AP_Character::CharacterTPL4);
    PlayerInputComponent->BindAction(TEXT("TimeControl"), EInputEvent::IE_Pressed, this, &AP_Character::CharacterTimeControl);
    PlayerInputComponent->BindAction(TEXT("TimeControl"), EInputEvent::IE_Released, this, &AP_Character::CharacterTimeControlB);
    PlayerInputComponent->BindAction(TEXT("TimeRecall"), EInputEvent::IE_Pressed, this, &AP_Character::CharacterTimeRecall);
    PlayerInputComponent->BindAction(TEXT("PickUp"), EInputEvent::IE_Pressed, this, &AP_Character::Interact);
    PlayerInputComponent->BindAction(TEXT("UsePotion"), EInputEvent::IE_Pressed, this, &AP_Character::UseHealP);
    PlayerInputComponent->BindAction(TEXT("MoveToMainLand"), EInputEvent::IE_Pressed, this, &AP_Character::MoveMain);
    PlayerInputComponent->BindAction(TEXT("Roll"), EInputEvent::IE_Pressed, this, &AP_Character::RollCharacter);
    PlayerInputComponent->BindAction(TEXT("TimeLock"), EInputEvent::IE_Pressed, this, &AP_Character::OnTimeLock);

    PlayerInputComponent->BindAction(TEXT("TimeControlWheelUp"), EInputEvent::IE_Pressed, this, &AP_Character::OnMouseWheelScroll);
    PlayerInputComponent->BindAction(TEXT("TimeControlWheelDown"), EInputEvent::IE_Pressed, this, &AP_Character::OnMouseWheelScrollDown);


    // 치트키
    PlayerInputComponent->BindAction(TEXT("Dash"), EInputEvent::IE_Pressed, this, &AP_Character::DashCheat);
    PlayerInputComponent->BindAction(TEXT("Dash"), EInputEvent::IE_Released, this, &AP_Character::DashCheatEnd);
    PlayerInputComponent->BindAction(TEXT("Exit"), EInputEvent::IE_Pressed, this, &AP_Character::exitGame);
    PlayerInputComponent->BindAction(TEXT("ResetSkill"), EInputEvent::IE_Pressed, this, &AP_Character::ResetSkill);
}


void AP_Character::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 otherBodyIndex, bool bRfromSweep, const FHitResult& Sweep)
{

}

void AP_Character::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (GEngine) {
    }
}

void AP_Character::EnableInteraction(AInteractableItem* Item)
{
    CurrentInteractableItem = Item;
}

void AP_Character::DisableInteraction()
{
    CurrentInteractableItem = nullptr;
}

void AP_Character::Interact()
{
    if (CurrentInteractableItem != nullptr)
    {
        AHealingPotion* HealingPotion = Cast<AHealingPotion>(CurrentInteractableItem);
        AFGem* FireGem = Cast<AFGem>(CurrentInteractableItem);
        ADGem* DesertGem = Cast<ADGem>(CurrentInteractableItem);
        if (HealingPotion)
        {
            CPotion++;
            CurrentInteractableItem->Destroy();
        }
        else if (FireGem) {
            fGemisGotten = true;
            CurrentInteractableItem->Destroy();
        }
        else if (DesertGem) {
            dGemisGotten = true;
            CurrentInteractableItem->Destroy();
        }
        else {
            // 이미 물건 들고 있는지 체크
            if (CurrentInteractableItem->ItemMesh->GetAttachParent() != GetMesh())
            {
                CurrentInteractableItem->OnInteract(GetRootComponent());
            }
            else
            {
            }
        }
    }
}



void AP_Character::UseHealP()
{
    UE_LOG(LogTemp, Warning, TEXT("UseHealP() called"));
    if (CPotion > 0) {
        CHP += 2;
        if (CHP > 10) {
            CHP = 10;
        }
        CPotion--;
    }

}

void AP_Character::RecallCountNsave()
{

    CRT = countRtime % 10;

    if (countRtime > 100000) {
        countRtime = 0;
    }

    countRtime += 1;

    switch (CRT) {
    case 0:
        RecallLocation[0] = GetActorLocation();
        SCHP[0] = CHP;
        CCC = 1;
        break;
    case 1:
        RecallLocation[1] = GetActorLocation();
        SCHP[1] = CHP;
        CCC = 2;
        break;
    case 2:
        RecallLocation[2] = GetActorLocation();
        SCHP[2] = CHP;
        CCC = 3;
        break;
    case 3:
        RecallLocation[3] = GetActorLocation();
        SCHP[3] = CHP;
        CCC = 4;
        break;
    case 4:
        RecallLocation[4] = GetActorLocation();
        SCHP[4] = CHP;
        CCC = 5;
        break;
    case 5:
        RecallLocation[5] = GetActorLocation();
        SCHP[5] = CHP;
        CCC = 6;
        break;
    case 6:
        RecallLocation[6] = GetActorLocation();
        SCHP[6] = CHP;
        CCC = 7;
        break;
    case 7:
        RecallLocation[7] = GetActorLocation();
        SCHP[7] = CHP;
        CCC = 8;
        break;
    case 8:
        RecallLocation[8] = GetActorLocation();
        SCHP[8] = CHP;
        CCC = 9;
        break;
    case 9:
        RecallLocation[9] = GetActorLocation();
        SCHP[9] = CHP;
        CCC = 1;
        break;
    default:
        break;
    }

    //여기서 걍 쿨타임도 처리
    if (canRecall == false) {
        RecallUse += 0.1f;
        if (RecallUse >= RecallCooldown) {
            RecallUse = 1.f;
            canRecall = true;
        }
    }
    if (CanControl == false) {
        ControlUse += 0.01f;
        if (ControlUse >= TimeControlCool) {
            ControlUse = 1.f;
            CanControl = true;
        }
    }
}


float AP_Character::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

    float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    return FinalDamage;
}



// 생성자 설정 빼옴
void AP_Character::SetCollision()
{

    // 콜리전
    CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionCapsule"));
    CollisionCapsule->InitCapsuleSize(50.f, 65.f); // 반지름, 높이
    CollisionCapsule->SetupAttachment(RootComponent);

    CollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &AP_Character::OnOverlapBegin);
    CollisionCapsule->OnComponentEndOverlap.AddDynamic(this, &AP_Character::OnOverlapEnd);

}

void AP_Character::SetCamera()
{

    // 카메라, 스프링암 추가
    m_pCamera = CreateDefaultSubobject<UCameraComponent>(L"Camera");
    m_pSpringArm = CreateDefaultSubobject<USpringArmComponent>(L"SpringArm");

    // 계층 구조 설정
    m_pSpringArm->SetupAttachment(GetMesh());
    m_pCamera->SetupAttachment(m_pSpringArm);

    // 스프링암 위치, 회전, 거리값 설정
    m_pSpringArm->SetRelativeLocation(FVector(0.f, 0.f, 900.f));
    m_pSpringArm->SetRelativeRotation(FRotator(-15.f, 90.f, 0.f));
    m_pSpringArm->TargetArmLength = 270.f;

    // 카메라 따로 회전 설정
    m_pSpringArm->bUsePawnControlRotation = true;
    m_pSpringArm->bInheritPitch = false;
    m_pSpringArm->bInheritRoll = true;
    m_pSpringArm->bInheritYaw = true;
    m_pSpringArm->bDoCollisionTest = true;
    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
}

void AP_Character::SetAsset()
{
    ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("SkeletalMesh'/Game/Rewind/Character/Main_Character/FBX/Main_Character.Main_Character'"));

    if (mesh.Succeeded())
    {
        GetMesh()->SetSkeletalMesh(mesh.Object);
        GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -89.f));
        GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
        GetMesh()->SetWorldScale3D(FVector(0.1f, 0.1f, 0.1f));
    }

    // 사용할 AnimInstance 설정
    ConstructorHelpers::FClassFinder<UAnimInstance> ABP(TEXT("AnimBlueprint'/Game/Rewind/Character/Main_Character/Animation/MC_AnimBP.MC_AnimBP_C'"));

    if (ABP.Succeeded())
    {
        GetMesh()->SetAnimInstanceClass(ABP.Class);
        UE_LOG(LogTemp, Warning, TEXT("Load Anim"));
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATMontage(TEXT("AnimMontage'/Game/Rewind/Character/Main_Character/Animation/ComboAttack'"));
    if (ATMontage.Succeeded())
    {
        AttackMontage = ATMontage.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> RMontage(TEXT("AnimMontage'/Game/Rewind/Character/Main_Character/Animation/RollMontage'"));
    if (RMontage.Succeeded())
    {
        RollMontage = RMontage.Object;
    }
}


// 캐릭터 기본 이동
void AP_Character::CharacterMoveFront(float _fScale)
{
    //// 기본 이동 속도 50
    //if (!isComboAttacking) {
    //    AddMovementInput(GetActorForwardVector(),
    //        50.f * GetWorld()->GetDeltaSeconds() * _fScale);
    //}
    //if (1.f == _fScale)
    //{
    //    if (90.f == m_AnimInst->GetDirection())
    //    {
    //        m_AnimInst->SetDirection(45.f);
    //    }
    //    else if (-90.f == m_AnimInst->GetDirection())
    //    {
    //        m_AnimInst->SetDirection(-45.f);
    //    }
    //    else
    //    {
    //        m_AnimInst->SetDirection(0.f);
    //    }
    //}
    //else if (-1.f == _fScale)
    //{
    //    if (90.f == m_AnimInst->GetDirection())
    //    {
    //        m_AnimInst->SetDirection(135.f);
    //    }
    //    else if (-90.f == m_AnimInst->GetDirection())
    //    {
    //        m_AnimInst->SetDirection(-135.f);
    //    }
    //    else
    //    {
    //        m_AnimInst->SetDirection(180.f);
    //    }
    //}

    if (0.f != _fScale) // W/D 가 눌렸다면 ( 이동키가 눌렸다면) 
    {
        if (!isJumping && !isComboAttacking) {
            ChangeState(EPLAYER_STATE::MOVE);
            AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), _fScale);
        }
    }
}

void AP_Character::CharacterMoveRight(float _fScale)
{
    /* if (!isComboAttacking) {
         AddMovementInput(GetActorRightVector(),
             50.f * GetWorld()->GetDeltaSeconds() * _fScale);
     }


     if (0.f == _fScale)
     {
         m_AnimInst->SetDirection(0.f);
     }
     else if (1.f == _fScale)
     {
         m_AnimInst->SetDirection(90.f);
     }
     else if (-1.f == _fScale)
     {
         m_AnimInst->SetDirection(-90.f);
     }*/

    if (0.f != _fScale) // W/D 가 눌렸다면 ( 이동키가 눌렸다면) 
    {
        if (!isJumping && !isComboAttacking) {
            ChangeState(EPLAYER_STATE::MOVE);
            AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), _fScale);
        }
    }
}

void AP_Character::CharacterRotationZ(float _fScale)
{
    AddControllerYawInput(_fScale);
}

void AP_Character::CharacterRotationY(float _fScale)
{
    FRotator rot = m_pSpringArm->GetRelativeRotation();
    float Pitch = rot.Pitch;
    Pitch += 100.f * GetWorld()->GetDeltaSeconds() * _fScale;

    if (Pitch < -45.f)
    {
        Pitch = -45.f;
    }
    else if (Pitch > 45.f)
    {
        Pitch = 45.f;
    }

    rot.Pitch = Pitch;
    m_pSpringArm->SetRelativeRotation(rot);

    rot.Roll;
    rot.Yaw;
}



void AP_Character::CharacterTimeControl()
{
    UE_LOG(LogTemp, Log, TEXT("TimeControl"));

    isTimeControlling = true;

    if (CanControl) {
        GetWorldSettings()->SetTimeDilation(0.2);

        AMainUI_PC* PC = Cast<AMainUI_PC>(GetController());
        if (PC != nullptr)
        {
            PC->OnToggleUIPressed();
        }
    }
}

void AP_Character::CharacterTimeControlB()
{
    isTimeControlling = false;

    GetWorldSettings()->SetTimeDilation(1.0);

    if (CanControl) {
        CanControl = false;
        ControlUse = 0.f;

        AMainUI_PC* PC = Cast<AMainUI_PC>(GetController());
        if (PC != nullptr)
        {
            PC->OnToggleUIReleased();
        }
    }

}

void AP_Character::CharacterTimeRecall()
{
    UE_LOG(LogTemp, Log, TEXT("TimeRecall"));
    if (canRecall) {

        CHP = SCHP[CCC];
        SetActorLocation(RecallLocation[CCC]);
        countRtime = 0;

        canRecall = false;
        RecallUse = 0.f;
    }
}

void AP_Character::CharacterTPL1()
{
    if (!S_Location1.IsEmpty()) {
        CharacterTPLocation(1);
        S_Location1.Reset();
        tp1 = 0.f;
    }
}

void AP_Character::CharacterTPL2()
{
    if (!S_Location2.IsEmpty()) {
        CharacterTPLocation(2);
        S_Location2.Reset();
        tp2 = 0.f;
    }
}

void AP_Character::CharacterTPL3()
{
    if (!S_Location3.IsEmpty()) {
        CharacterTPLocation(3);
        S_Location3.Reset();
        tp3 = 0.f;
    }
}

void AP_Character::CharacterTPL4()
{
    if (!S_Location4.IsEmpty()) {
        CharacterTPLocation(4);
        S_Location4.Reset();
        tp4 = 0.f;
    }
}

void AP_Character::CharacterSaveLocation()
{
    UE_LOG(LogTemp, Log, TEXT("SaveLocation"));

    SaveCurLocation();

}

void AP_Character::CharacterTPLocation(int SN)
{
    UE_LOG(LogTemp, Log, TEXT("TP"));

    int count = 0;

    if (SN == 1) {
        tp1AIMG->DestroyAA(0.001f);
        SetActorLocation(S_Location1[0]);
    }
    else if (SN == 2) {
        tp2AIMG->DestroyAA(0.001f);
        SetActorLocation(S_Location2[0]);
    }
    else if (SN == 3) {
        tp3AIMG->DestroyAA(0.001f);
        SetActorLocation(S_Location3[0]);
    }
    else {
        tp4AIMG->DestroyAA(0.001f);
        SetActorLocation(S_Location4[0]);
    }
}

void AP_Character::CharacterJump()
{
    //    UE_LOG(LogTemp, Log, TEXT("Jump called"));

        //ChangeState(EPLAYER_STATE::JUMP);
    Jump();

}

void AP_Character::MoveMain()
{
    UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("MainLand")));
}

void AP_Character::SaveCurPose() // AfterIMG
{
    FActorSpawnParameters param = {};
    param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    param.OverrideLevel = GetLevel();
    param.bDeferConstruction = false;

    if (tp1 < 0.5f) {
        tp1AIMG = GetWorld()->SpawnActor<AAfterIMG>(AAfterIMG::StaticClass()
            , GetMesh()->GetComponentLocation()
            , GetMesh()->GetComponentRotation()
            , param);
        tp1AIMG->SetSkeletalMeshComponent(GetMesh());
    }
    else if (tp2 < 0.5f) {
        tp2AIMG = GetWorld()->SpawnActor<AAfterIMG>(AAfterIMG::StaticClass()
            , GetMesh()->GetComponentLocation()
            , GetMesh()->GetComponentRotation()
            , param);
        tp2AIMG->SetSkeletalMeshComponent(GetMesh());
    }
    else if (tp3 < 0.5f) {
        tp3AIMG = GetWorld()->SpawnActor<AAfterIMG>(AAfterIMG::StaticClass()
            , GetMesh()->GetComponentLocation()
            , GetMesh()->GetComponentRotation()
            , param);
        tp3AIMG->SetSkeletalMeshComponent(GetMesh());
    }
    else if (tp4 < 0.5f) {
        tp4AIMG = GetWorld()->SpawnActor<AAfterIMG>(AAfterIMG::StaticClass()
            , GetMesh()->GetComponentLocation()
            , GetMesh()->GetComponentRotation()
            , param);
        tp4AIMG->SetSkeletalMeshComponent(GetMesh());
    }

}

void AP_Character::ComboAttackDown()
{
    UE_LOG(LogTemp, Warning, TEXT("ComboAttackDown"));
    isComboAttackDown = true;

    if (isComboAttacking == false) {
        ComboAttack();
    }
    else if (isComboAttacking == true) {
        isComboAttackNext = true;
    }
}

void AP_Character::ComboAttackUp()
{
    isComboAttackDown = false;
}

void AP_Character::ComboAttack()
{
    UE_LOG(LogTemp, Warning, TEXT("ComboAttack"));
    isComboAttackDown = true;

    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (!AnimInstance || !AttackMontage) {
        UE_LOG(LogTemp, Warning, TEXT("WWComboAttack"));
        return;
    }

    isComboAttacking = true;
    const char* ComboList[] = { "Attack1","Attack2", "Attack3" };

    if (ComboAttackCount >= 3) {
        ComboAttackCount = 0;
    }
    UE_LOG(LogTemp, Warning, TEXT("ComboAttack%d"), ComboAttackCount);

    AnimInstance->Montage_Play(AttackMontage, 1.5f);
    AnimInstance->Montage_JumpToSection(FName(ComboList[ComboAttackCount]), AttackMontage);
}

void AP_Character::ComboAttackEnd()
{
    UE_LOG(LogTemp, Warning, TEXT("ComboAttackEnd"));
    isComboAttacking = false;
    ComboAttackCount = 0;
}

void AP_Character::ComboAttackCheck()
{
    UE_LOG(LogTemp, Warning, TEXT("ComboAttackCheck"));

    if (isComboAttackNext == true) {
        ComboAttackCount += 1;
        isComboAttackNext = false;
        ComboAttack();
    }
}

void AP_Character::EnableWeaponCollision()
{
    if (Weapon)
    {
        UE_LOG(LogTemp, Warning, TEXT("EnableWeaponCollision"))
            Weapon->EnableCollision();
    }
}

void AP_Character::DisableWeaponCollision()
{
    if (Weapon)
    {
        UE_LOG(LogTemp, Warning, TEXT("DisableWeaponCollision"))
            Weapon->DisableCollision();
    }
}


void AP_Character::SaveCurLocation()
{
    if (S_Location1.IsEmpty()) {
        S_Location1.Add(GetActorLocation());
        SaveCurPose();
        tp1 = 1.f;

    }
    else if (S_Location2.IsEmpty()) {
        S_Location2.Add(GetActorLocation());
        SaveCurPose();
        tp2 = 1.f;
    }
    else if (S_Location3.IsEmpty()) {
        S_Location3.Add(GetActorLocation());
        SaveCurPose();
        tp3 = 1.f;
    }
    else if (S_Location4.IsEmpty()) {
        S_Location4.Add(GetActorLocation());
        SaveCurPose();
        tp4 = 1.f;
    }
}

void AP_Character::setCharacterState(int32 NewPotionCount, int32 NewCHP, float NewRecallUse, float NewControlUse, bool NewFGEMGet, bool NewDGEMGet, bool NewIGEMGet, bool NewMGEMGet)
{
    CPotion = NewPotionCount;
    CHP = NewCHP;
    RecallUse = NewRecallUse;
    ControlUse = NewControlUse;

    fGemisGotten = NewFGEMGet;
    dGemisGotten = NewDGEMGet;
    iGemisGotten = NewIGEMGet;
    mGemisGotten = NewMGEMGet;

}

void AP_Character::OnMouseWheelScroll()
{

    //UE_LOG(LogTemp, Warning, TEXT("Mouse wheel scroll value: %f"), Value);
    if (isTimeControlling && NearestMonster)
    {
        AgeValue += 1.f;
        //UE_LOG(LogTemp, Warning, TEXT("exist"));
        UE_LOG(LogTemp, Warning, TEXT("Scroll Value: %f"), AgeValue);
        AgeMonster(AgeValue);
    }
}

void AP_Character::OnMouseWheelScrollDown()
{
    //UE_LOG(LogTemp, Warning, TEXT("Mouse wheel scroll value: %f"), Value);
    if (isTimeControlling && NearestMonster)
    {
        AgeValue -= 1.f;
        //UE_LOG(LogTemp, Warning, TEXT("exist"));
        //UE_LOG(LogTemp, Warning, TEXT("Scroll Value: %f"), AgeValue);
        AgeMonster(AgeValue);
    }
}

void AP_Character::UpdateNearestMonster()
{
    TArray<AActor*> FoundMonsters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMonster::StaticClass(), FoundMonsters);

    float NearestDistance = FLT_MAX;
    AMonster* Nearest = nullptr;

    for (AActor* Actor : FoundMonsters)
    {
        AMonster* Monster = Cast<AMonster>(Actor);
        if (Monster)
        {
            float Distance = FVector::Dist(this->GetActorLocation(), Monster->GetActorLocation());
            if (Distance < NearestDistance)
            {
                NearestDistance = Distance;
                Nearest = Monster;
            }
        }
    }

    NearestMonster = Nearest;
}

void AP_Character::AgeMonster(float Value)
{
    if (NearestMonster)
    {
        NearestMonster->ChangeAge(Value);
    }
}

void AP_Character::RollCharacter()
{
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

    AnimInstance->Montage_Play(RollMontage, 0.9f);

    if (AnimInstance && AnimInstance->Montage_Play(RollMontage, 1.5f))
    {
        GetWorld()->GetTimerManager().SetTimer(RollTimerHandle, this, &AP_Character::RollMove, 0.01f, true);
    }
}

void AP_Character::RollMove()
{
    const FVector RollDirection = GetActorForwardVector();
    AddMovementInput(RollDirection, 1.0f);

    if (!GetMesh()->GetAnimInstance()->Montage_IsPlaying(RollMontage))
    {
        GetWorld()->GetTimerManager().ClearTimer(RollTimerHandle);
    }
}


//가장 가까운 타임록액터 서치
void AP_Character::UpdateNearestTLActor()
{
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATimeLockableActor::StaticClass(), FoundActors);

    float NearestDistance = FLT_MAX;
    ATimeLockableActor* Nearest = nullptr;

    for (AActor* Actor : FoundActors)
    {
        ATimeLockableActor* TLA = Cast<ATimeLockableActor>(Actor);
        if (TLA)
        {
            //UE_LOG(LogTemp, Warning, TEXT("Find Actor"));
            float Distance = FVector::Dist(this->GetActorLocation(), TLA->GetActorLocation());
            if (Distance < NearestDistance)
            {
                NearestDistance = Distance;
                Nearest = TLA;
            }
        }
    }

    NearestActor1 = Nearest;
}

void AP_Character::OnTimeLock()
{
    isTimeLocking = true;

    UpdateNearestTLActor();
    UpdateNearestMonster();


    AActor* NearestActor = nullptr;

    if (NearestMonster && NearestActor1) {
        float DistanceToMonster = FVector::Dist(this->GetActorLocation(), NearestMonster->GetActorLocation());
        float DistanceToActor = FVector::Dist(this->GetActorLocation(), NearestActor1->GetActorLocation());
        if (DistanceToMonster < DistanceToActor)
        {
            MMTimeLock();
        }
        else
        {
            AATimeLock();
        }
    }
    else if (NearestMonster)
    {
        MMTimeLock();
    }
    else if (NearestActor1)
    {
        AATimeLock();
    }

    //if (NearestActor1 && isTimeLocking)
    //{
    //    UE_LOG(LogTemp, Warning, TEXT("Find Actor"));
    //    NearestActor1->bIsTimeLocked = true; // 이거 필요한가?
    //    NearestActor1->StartTimeLock();
    //    GetWorld()->GetTimerManager().SetTimer(TimeLockHandle, [this]()
    //        {
    //            this->NearestActor1->bIsTimeLocked = false;
    //            this->NearestActor1->ApplyAccumulatedDamage();
    //            OnTimeLockEnd();
    //        }, 5.0f, false);
    //}
}

void AP_Character::OnTimeLockEnd()
{
    isTimeLocking = false;
}

void AP_Character::AATimeLock()
{

    if (NearestActor1 && isTimeLocking)
    {
        UE_LOG(LogTemp, Warning, TEXT("Find Actor"));
        NearestActor1->bIsTimeLocked = true; // 이거 필요한가?
        NearestActor1->StartTimeLock();

        GetWorld()->GetTimerManager().SetTimer(TimeLockHandle, [this]()
            {
                this->NearestActor1->bIsTimeLocked = false;
                this->NearestActor1->ApplyAccumulatedDamage();
                OnTimeLockEnd();
            }, 5.0f, false);
    }
}

void AP_Character::MMTimeLock()
{
    if (NearestMonster && isTimeLocking)
    {
        UE_LOG(LogTemp, Warning, TEXT("Find Actor"));
        NearestMonster->bIsTimeLocked = true; // 이거 필요한가?
        NearestMonster->StartTimeLock();

        GetWorld()->GetTimerManager().SetTimer(TimeLockHandle, [this]()
            {
                this->NearestMonster->bIsTimeLocked = false;
                this->NearestMonster->ApplyAccumulatedDamage();
                OnTimeLockEnd();
            }, 5.0f, false);
    }
}

void AP_Character::DashCheat()
{
    GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
}


void AP_Character::DashCheatEnd()
{
    GetCharacterMovement()->MaxWalkSpeed = 500.0f;
}

void AP_Character::exitGame()
{
    GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
}


void AP_Character::ResetSkill()
{
    RecallUse = 1.f;
    ControlUse = 1.f;
}

