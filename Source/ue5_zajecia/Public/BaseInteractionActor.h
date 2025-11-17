// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"
#include "BaseInteractionActor.generated.h"

UCLASS()
class UE5_ZAJECIA_API ABaseInteractionActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	ABaseInteractionActor();
public:	
	virtual void Interact(AActor* Interactor) override;
	virtual void PickUp(AActor* Interactor);
};
