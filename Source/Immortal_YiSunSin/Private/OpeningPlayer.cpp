// Fill out your copyright notice in the Description page of Project Settings.


#include "OpeningPlayer.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include <HeadMountedDisplayFunctionLibrary.h>
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h>
#include <MotionControllerComponent.h>
#include <Camera/CameraComponent.h>
#include "PickupActor.h"
#include <Components/BoxComponent.h>
#include <Components/SkeletalMeshComponent.h>



// Sets default values
AOpeningPlayer::AOpeningPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	cam = CreateDefaultSubobject<UCameraComponent>("Cam");
	cam->SetupAttachment(RootComponent);

	headMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Head Mesh"));
	headMesh->SetupAttachment(cam);
	headMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	controllerLeft = CreateDefaultSubobject<UMotionControllerComponent>("Left Controller");
	controllerLeft->SetupAttachment(RootComponent);
	controllerLeft->MotionSource = "Left";

	leftHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Left Hand"));
	leftHand->SetupAttachment(controllerLeft);
	leftHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	controllerRight = CreateDefaultSubobject<UMotionControllerComponent>("Right Controller");
	controllerRight->SetupAttachment(RootComponent);
	controllerRight->MotionSource = "Right";

	rightHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Right Hand"));
	rightHand->SetupAttachment(controllerRight);
	rightHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);


}

// Called when the game starts or when spawned
void AOpeningPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(trackOrigin.GetValue());

	APlayerController* playerCon = GetWorld()->GetFirstPlayerController();

	UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerCon->GetLocalPlayer());

	subsys->AddMappingContext(inputMapping, 0);

}

// Called every frame
void AOpeningPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AOpeningPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	enhancedInputComponent->BindAction(thumbstickLeft, ETriggerEvent::Triggered, this, &AOpeningPlayer::Move);
	enhancedInputComponent->BindAction(thumbstickRight, ETriggerEvent::Triggered, this, &AOpeningPlayer::RotateAxis);

}

void AOpeningPlayer::Recenter()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AOpeningPlayer::Move(const struct FInputActionValue& value)
{

	FVector2D val = value.Get<FVector2D>();
	//FVector direction = FVector(GetActorForwardVector()*val.Y,GetActorRightVector()*val.X, 0);
	//�÷��̾� �𷺼����� �̵�
	FVector dir = GetActorForwardVector() * val.Y + GetActorRightVector() * val.X;
	AddMovementInput(dir.GetSafeNormal(), 1, false);
}

void AOpeningPlayer::RotateAxis(const struct FInputActionValue& value)
{
	FVector2D axis = value.Get<FVector2D>();

	//axis ���� �̿��ؼ� ĳ����(��Ʈ�ѷ�)�� ȸ���Ѵ�.
	AddControllerYawInput(axis.X);
}
