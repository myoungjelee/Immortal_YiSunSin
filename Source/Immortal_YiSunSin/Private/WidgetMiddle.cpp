// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetMiddle.h"
#include <Components/BoxComponent.h>
#include <UMG/Public/Components/WidgetComponent.h>
#include "EasingLibrary.h"
#include "mainPlayer.h"
#include <Kismet/GameplayStatics.h>
#include <Components/AudioComponent.h>

// Sets default values
AWidgetMiddle::AWidgetMiddle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	box = CreateDefaultSubobject<UBoxComponent>("Box");
	SetRootComponent(box);

	widget = CreateDefaultSubobject<UWidgetComponent>("Widget");
	widget->SetupAttachment(box);
	widget->SetVisibility(false);
	//widget->SetRelativeLocation(FVector(0, 0, 250));
	widget->SetCollisionProfileName(TEXT("interactionUI"));
	
}

// Called when the game starts or when spawned
void AWidgetMiddle::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AmainPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AmainPlayer::StaticClass()));
	
	startPos = GetActorLocation();
	endPos = GetActorLocation() + GetActorRightVector()*100;
}

// Called every frame
void AWidgetMiddle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ((GetActorLocation() - player->GetActorLocation()).Length() >= 450)
	{
		widget->SetVisibility(false);

		if (audio != nullptr)
		{
			audio->Stop();
		}
	}

	if (open)
	{

		if (soundState == 0)
		{
			audio = UGameplayStatics::SpawnSound2D(GetWorld(), infoSound);
			soundState++;
		}


		widget->SetVisibility(true);
		//widget->SetCollisionProfileName(TEXT("interactionUI"));

		currTime += DeltaTime;
		param = FMath::Clamp(currTime * 1.8f, 0, 1);
		float easy = UEasingLibrary::BackEaseOut(param);

		FVector newLoc = FMath::Lerp(endPos, startPos, easy);

		SetActorLocation(newLoc);

		if (param == 1)
		{
			currTime = 0;
			soundState = 0;
			open = false;
		}
	}
}

