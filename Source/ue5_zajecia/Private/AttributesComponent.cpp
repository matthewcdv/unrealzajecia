#include "AttributesComponent.h"

UAttributesComponent::UAttributesComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MaxHealth = 100.0f;
	MaxStamina = 100.0f;

	Health = MaxHealth;
	Stamina = MaxStamina;
}


void UAttributesComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	Stamina = MaxStamina;

	OnHealthChanged.Broadcast(Health, MaxHealth);
	OnStaminaChanged.Broadcast(Stamina, MaxStamina);
}

void UAttributesComponent::ApplyDamage(float Damage)
{
	if (Health <= 0.0f || Damage <= 0.0f)
	{
		return;
	}
	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("Aktor %s received %f damage, health: %f"), *GetOwner()->GetName(), Damage, Health);
	OnHealthChanged.Broadcast(Health, MaxHealth);

	if (Health == 0.0f)
	{
		OnDeath.Broadcast();
	}
}

void UAttributesComponent::SetHealth(float NewHealth)
{
	Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	if (Health == 0.0f)
	{
		OnDeath.Broadcast();
	}
}

void UAttributesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Stamina < MaxStamina)
	{
		float RegenAmount = StaminaCosts.StaminaRegenRate * DeltaTime;

		SetStamina(Stamina + RegenAmount);
	}
}

void UAttributesComponent::SetStamina(float NewStamina)
{
	float ClampedStamina = FMath::Clamp(NewStamina, 0.0f, MaxStamina);

	if (Stamina != ClampedStamina)
	{
		Stamina = ClampedStamina;
		OnStaminaChanged.Broadcast(Stamina, MaxStamina);
	}
}

bool UAttributesComponent::CanPayStaminaCost(float Cost)
{
	return Stamina >= Cost;
}

void UAttributesComponent::PayStaminaCost(float Cost)
{
	if (CanPayStaminaCost(Cost))
	{
		SetStamina(Stamina - Cost);
	}
}