// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enum/PawnState.h"
#include "MainHUD.generated.h"

class UProgressBar;
class UAttributesComponent;
class UTextBlock;

UCLASS()
class UE5_ZAJECIA_API UMainHUD : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* StateText;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* EnemyHealthBar;

	void UpdateEnemyHealth(float CurrentHealth, float MaxHealth);

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void UpdateHealth(float CurrentHealth, float MaxHealth);

	UFUNCTION()
	void UpdateStamina(float CurrentStamina, float MaxStamina);

	UFUNCTION()
	void UpdateStateText(EPawnState NewState);

private:
	FTimerHandle HideEnemyBarTimer;

	void HideEnemyHealth();
};
