// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ABasePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UE5_ZAJECIA_API AABasePlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget> HUDClass;

	UPROPERTY()
	class UUserWidget* HUDInstance;
};
