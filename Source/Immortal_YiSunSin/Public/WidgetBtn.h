// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WidgetBtn.generated.h"

UCLASS()
class IMMORTAL_YISUNSIN_API AWidgetBtn : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWidgetBtn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UWidgetComponent* widget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AmainPlayer* player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UYiSunSinInstance* instance;
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool near;

};