#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributesComponent.generated.h"

USTRUCT(BlueprintType)
struct FStaminaCost
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaRegenRate = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaCost_Attack = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaCost_Sprint = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaCost_Jump = 20.0f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeChanged, float, CurrentValue, float, MaxValue);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5_ZAJECIA_API UAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributesComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDeathDelegate OnDeath;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void ApplyDamage(float Damage);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void SetHealth(float NewHealth);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnAttributeChanged OnStaminaChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnAttributeChanged OnHealthChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	FStaminaCost StaminaCosts;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetStamina() const { return Stamina; }

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxStamina() const { return MaxStamina; }

	void SetStamina(float NewStamina);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool CanPayStaminaCost(float Cost);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void PayStaminaCost(float Cost);
protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float MaxStamina = 100.0f;

private:
	UPROPERTY(VisibleAnywhere, Category = "Attributes")
	float Health;

	UPROPERTY(VisibleAnywhere, Category = "Attributes")
	float Stamina;
	
};
