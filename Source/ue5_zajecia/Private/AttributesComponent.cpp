#include "AttributesComponent.h"

UAttributesComponent::UAttributesComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UAttributesComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
}

void UAttributesComponent::ApplyDamage(float Damage)
{
	if (Health <= 0.0f || Damage <= 0.0f)
	{
		return;
	}
	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("Aktor %s received %f damage, health: %f"), *GetOwner()->GetName(), Damage, Health);

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