// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/MainHUD.h"
#include "Components/ProgressBar.h"
#include "AttributesComponent.h"
#include "Components/TextBlock.h"
#include "ABasePlayerCharacter.h"

void UMainHUD::NativeConstruct()
{
	Super::NativeConstruct();

    if (EnemyHealthBar)
    {
        EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
    }

	if (APawn* OwningPawn = GetOwningPlayerPawn())
	{
		if (AABasePlayerCharacter* PlayerChar = Cast<AABasePlayerCharacter>(OwningPawn))
		{
			if (UAttributesComponent* Attributes = PlayerChar->AttributesComponent)
			{
				Attributes->OnHealthChanged.AddDynamic(this, &UMainHUD::UpdateHealth);
				Attributes->OnStaminaChanged.AddDynamic(this, &UMainHUD::UpdateStamina);
				PlayerChar->OnStateChanged.AddDynamic(this, &UMainHUD::UpdateStateText);

				UpdateHealth(Attributes->GetHealth(), Attributes->GetMaxHealth());
				UpdateStamina(Attributes->GetStamina(), Attributes->GetMaxStamina());
				UpdateStateText(PlayerChar->GetCurrentState());
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

void UMainHUD::UpdateStateText(EPawnState NewState)
{
    if (!StateText) return;

    FString StateString = TEXT("Unknown");

    switch (NewState)
    {
    case EPawnState::EPS_Idle:
        StateString = TEXT("IDLE");
        StateText->SetColorAndOpacity(FLinearColor::White);
        break;
    case EPawnState::EPS_InCombat:
        StateString = TEXT("COMBAT");
        StateText->SetColorAndOpacity(FLinearColor::Yellow);
        break;
    case EPawnState::EPS_Occupied:
        StateString = TEXT("ATTACKING");
        StateText->SetColorAndOpacity(FLinearColor::Blue);
        break;
    case EPawnState::EPS_Hit:
        StateString = TEXT("HIT!");
        StateText->SetColorAndOpacity(FLinearColor::Red);
        break;
    case EPawnState::EPS_Exhausted:
        StateString = TEXT("EXHAUSTED");
        StateText->SetColorAndOpacity(FLinearColor::White);
        break;
    case EPawnState::EPS_Dead:
        StateString = TEXT("DEAD");
        StateText->SetColorAndOpacity(FLinearColor::Black);
        break;
    }

    StateText->SetText(FText::FromString(StateString));
}

void UMainHUD::UpdateEnemyHealth(float CurrentHealth, float MaxHealth)
{
	if (EnemyHealthBar)
	{
		if (MaxHealth > 0.0f)
		{
			float Percent = CurrentHealth / MaxHealth;
			EnemyHealthBar->SetPercent(Percent);
		}

		EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);

		if (GetWorld())
		{
			GetWorld()->GetTimerManager().ClearTimer(HideEnemyBarTimer);
			GetWorld()->GetTimerManager().SetTimer(
				HideEnemyBarTimer,
				this,
				&UMainHUD::HideEnemyHealth,
				3.0f,
				false
			);
		}
	}
}

void UMainHUD::HideEnemyHealth()
{
	if (EnemyHealthBar)
	{
		EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
	}
}