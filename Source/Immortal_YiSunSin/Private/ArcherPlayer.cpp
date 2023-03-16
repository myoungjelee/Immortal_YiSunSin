// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "MotionControllerComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include <GameFramework/PlayerController.h>
#include "ArrowActor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "EngineUtils.h"

AArcherPlayer::AArcherPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Root Comp"));
	SetRootComponent(rootComp);

	cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	cam->SetupAttachment(RootComponent);

	leftController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Left Controller"));
	leftController->SetupAttachment(RootComponent);
	leftController->MotionSource = "Left";

	leftHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Left Hand"));
	leftHand->SetupAttachment(leftController);
	leftHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	leftHand->SetRelativeRotation(FRotator(-25.0f, 180.0f, 90.0f));

	rightController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Right Controller"));
	rightController->SetupAttachment(RootComponent);
	rightController->MotionSource = "Right";

	rightHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Right Hand"));
	rightHand->SetupAttachment(rightController);
	rightHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	rightHand->SetRelativeRotation(FRotator(25.0f, 0.0f, 90.0f));

	bowComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bow Mesh"));
	bowComp->SetupAttachment(leftHand);
	bowComp->SetRelativeLocation(FVector(0, 2.2, -3.84));
	bowComp->SetRelativeRotation(FRotator(0, -90, 0));

	handleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Handle Mesh"));
	handleMesh->SetupAttachment(bowComp);
	handleMesh->SetRelativeLocation(FVector(14.5, 0, 0.12));
	handleMesh->SetRelativeRotation(FRotator(0, 180, 0));
	handleMesh->SetRelativeScale3D(FVector(0.05));
}

void AArcherPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(trackOrigin.GetValue());

	APlayerController* playerCon = GetWorld()->GetFirstPlayerController();

	UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerCon->GetLocalPlayer());

	subsys->AddMappingContext(myMapping, 0);

	handleMesh->SetVisibility(false);

	startLoc = handleMesh->GetComponentLocation();
	tempLoc = handleMesh->GetRelativeLocation();
}

void AArcherPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector temp = rightHand->GetComponentLocation() - startLoc;
	handLoc = rightHand->GetComponentLocation();

	if (temp.Length() < 150)
	{
		if (bBowPulling == true)
		{
			handleMesh->SetWorldLocation(handLoc);
			arrow->SetActorLocation(handleMesh->GetComponentLocation() + arrow->GetActorForwardVector() * 35);
			arrow->SetActorRotation(handleMesh->GetComponentRotation());
		}
	}
}

void AArcherPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (enhancedInputComponent != nullptr)
	{
		enhancedInputComponent->BindAction(GripRight, ETriggerEvent::Started, this, &AArcherPlayer::BowRelease);
		enhancedInputComponent->BindAction(GripRight, ETriggerEvent::Completed, this, &AArcherPlayer::ShootArrow);
	}
}

void AArcherPlayer::BowRelease()
{
	bBowPulling = true;

	// ȭ�� ���� --
	FVector shootLoc = handleMesh->GetComponentLocation();
	FRotator shootRot = handleMesh->GetComponentRotation();
	arrow = Cast<AArrowActor>(GetWorld()->SpawnActor<AArrowActor>(arrowFactory, shootLoc, shootRot));
	arrow->projectileComp->SetActive(!bBowPulling);
}

void AArcherPlayer::ShootArrow()
{
	bBowPulling = false;
	handleMesh->SetRelativeLocation(tempLoc);

	arrow->Shoot();
}
