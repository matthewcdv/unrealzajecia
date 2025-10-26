// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseInteractionActor.h"

// Sets default values
ABaseInteractionActor::ABaseInteractionActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseInteractionActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseInteractionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

