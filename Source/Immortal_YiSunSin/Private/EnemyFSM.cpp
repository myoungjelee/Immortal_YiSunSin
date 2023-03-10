// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include <AIModule/Classes/AIController.h>
#include "playerMG4.h"
#include "EnemyShip.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
#include <NavigationSystem.h>
#include <Components/CapsuleComponent.h>

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();
	currHp = maxHp;

	target = Cast<AplayerMG4>(UGameplayStatics::GetActorOfClass(GetWorld(),AplayerMG4::StaticClass()));
	me = Cast<AEnemyShip>(GetOwner());

	ai = Cast<AAIController>(me->GetController());

}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (currState)
	{
	case EEnemyState::Idle:
		UpdateIdle();
		break;
	case EEnemyState::Move:
		UpdateMove();
		break;
	case EEnemyState::Damaged:
		UpdateDamaged();
		break;
	case EEnemyState::Die:
		UpdateDie();
		break;
	}
}

void UEnemyFSM::UpdateIdle()
{
	if (IsTargetTrace())
	{
		ChangeState(EEnemyState::Move);
	}
	else
	{
		if (IsWaitComplete(idleDelayTime))
		{
			ChangeState(EEnemyState::Move);
		}
	}
}

void UEnemyFSM::UpdateMove()
{
	bool bTrace = IsTargetTrace();

	FVector dir = target->GetActorLocation() - me->GetActorLocation();
	float dist = FVector::Distance(originPos, me->GetActorLocation());

	if (bTrace) //���� �þ߿� ���Դٸ�
	{
		ai->MoveToLocation(target->GetActorLocation());
	}
	else
	{
		MoveToPos(randPos);
	}
}

void UEnemyFSM::UpdateDamaged()
{
	if (IsWaitComplete(damageDelayTime))
	{
		ChangeState(EEnemyState::Idle);
	}
}

void UEnemyFSM::UpdateDie()
{
	if (bDieMove == false) return;
	FVector p0 = me->GetActorLocation();
	FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	FVector p = p0 + vt;
	
	if (p.Z < -200)
	{
		me->Destroy();
	}
	else
	{
		me->SetActorLocation(p);
	}
}



void UEnemyFSM::ChangeState(EEnemyState state)
{
	//���� ���¸� ����
	currState = state;

	//���� �ð� �ʱ�ȭ
	currTime = 0;

	//ai �� ������ ������
	ai->StopMovement();

	//���¿� ���� �ʱ⼳��
	switch (currState)
	{
	case EEnemyState::Move:
	{
		//�׺���̼� �ý��� ��������
		UNavigationSystemV1* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		//������ ��ġ�� �����
		FNavLocation loc;
		ns->GetRandomReachablePointInRadius(originPos, 1000, loc);
		randPos = loc.Location;
	}
	break;
	case EEnemyState::Die:
		//�浹�ȵǰ� ����
		me->GetCapsuleComponent()->SetCollisionEnabled (ECollisionEnabled::NoCollision);
	break;
	}
}

void UEnemyFSM::ReceiveDamage()
{
	currHp--;
	if (currHp > 0)
	{
		ChangeState(EEnemyState::Damaged);
	}
	else
	{
		ChangeState(EEnemyState::Die);
	}
}

bool UEnemyFSM::IsWaitComplete(float delayTime)
{
	currTime += GetWorld()->DeltaTimeSeconds;

	if (currTime > delayTime)
	{
		currTime = 0;
		return true;
	}
	return false;
}

bool UEnemyFSM::IsTargetTrace()
{
	FVector dir = target->GetActorLocation() - me->GetActorLocation();

	float dotValue = FVector::DotProduct(me->GetActorForwardVector(), dir.GetSafeNormal());
	float angle = UKismetMathLibrary::DegAcos(dotValue);

	//30��(�þ߰�60) ���� �۰� traceRange ���� �Ÿ��� ���� ��
	if (angle<30 && dir.Length() < traceRange)
	{
		FHitResult hitInfo;
		FCollisionQueryParams param;
		param.AddIgnoredActor(me);

		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, me->GetActorLocation(), target->GetActorLocation(), ECollisionChannel::ECC_Visibility, param);

		if (bHit)
		{
			if (hitInfo.GetActor()->GetName().Contains(TEXT("player")))
			{
				return true;
			}
		}
	}
	return false;
}

void UEnemyFSM::MoveToPos(FVector pos)
{
	//�ش� ��ġ(pos) �� ����
	EPathFollowingRequestResult::Type result = ai->MoveToLocation(pos);

	//���࿡ �������� �����ߴٸ�
	if (result == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		//Idle ���·� ��ȯ
		ChangeState(EEnemyState::Idle);
	}
}