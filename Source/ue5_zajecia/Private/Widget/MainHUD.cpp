// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/MainHUD.h"
#include "Components/ProgressBar.h"
#include "AttributesComponent.h"
#include "ABasePlayerCharacter.h"

void UMainHUD::NativeConstruct()
{
	Super::NativeConstruct();

	// Znajdü gracza i jego komponent
	if (APawn* OwningPawn = GetOwningPlayerPawn())
	{
		if (AABasePlayerCharacter* PlayerChar = Cast<AABasePlayerCharacter>(OwningPawn))
		{
			if (UAttributesComponent* Attributes = PlayerChar->AttributesComponent)
			{
				Attributes->OnHealthChanged.AddDynamic(this, &UMainHUD::UpdateHealth);
				Attributes->OnStaminaChanged.AddDynamic(this, &UMainHUD::UpdateStamina);

				UpdateHealth(Attributes->GetHealth(), Attributes->GetMaxHealth());
				UpdateStamina(Attributes->GetStamina(), Attributes->GetMaxStamina());
			}
		}
	}
}

void UMainHUD::UpdateHealth(float CurrentHealth, float MaxHealth)
{
	if (HealthBar && MaxHealth > 0.0f)
	{
		float Percent = CurrentHealth / MaxHealth;
		HealthBar->SetPercent(Percent);
	}
}

void UMainHUD::UpdateStamina(float CurrentStamina, float MaxStamina)
{
	if (StaminaBar && MaxStamina > 0.0f)
	{
		float Percent = CurrentStamina / MaxStamina;
		StaminaBar->SetPercent(Percent);
	}
}