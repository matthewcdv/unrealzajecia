// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributesComponent.h"

// Sets default values for this component's properties
UAttributesComponent::UAttributesComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UAttributesComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Health = MaxHealth;
}


// Called every frame
void UAttributesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAttributesComponent::ApplyDamage(float Damage)
{
	if (Health <= 0.0f || Damage <= 0.0f)
	{
		return;
	}

	// Odejmij HP
	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("Aktor %s received %f damage, health: %f"), *GetOwner()->GetName(), Damage, Health);

	// Sprawdü, czy to by≥ úmiertelny cios
	if (Health == 0.0f)
	{
		OnDeath.Broadcast(); // Wywo≥aj delegat úmierci
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