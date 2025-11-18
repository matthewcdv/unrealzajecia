// Fill out your copyright notice in the Description page of Project Settings.


#include "ABasePlayerController.h"
#include "Blueprint/UserWidget.h"

void AABasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HUDClass)
	{
		if (IsLocalController())
		{
			HUDInstance = CreateWidget<UUserWidget>(this, HUDClass);

			if (HUDInstance)
			{
				HUDInstance->AddToViewport();
			}
		}
	}
}
