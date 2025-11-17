// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseInteractionActor.h"

// Sets default values
ABaseInteractionActor::ABaseInteractionActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseInteractionActor::Interact(AActor* Interactor)
{
	PickUp(Interactor);
}

void ABaseInteractionActor::PickUp(AActor* Interactor)
{

}

