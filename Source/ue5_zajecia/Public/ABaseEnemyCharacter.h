#pragma once

#include "CoreMinimal.h"
#include "ABaseCharacter.h"
#include "CombatInterface.h"
#include "Enum/PawnState.h"
// USUNIÊTO: #include "Perception/PawnSensingComponent.h" (Ju¿ niepotrzebne)
#include "ABaseEnemyCharacter.generated.h"

class APickableWeapon;

UCLASS()
class UE5_ZAJECIA_API AABaseEnemyCharacter : public AABaseCharacter, public ICombatInterface
{
	GENERATED_BODY()

public:
	AABaseEnemyCharacter();

	virtual void GetHit_Implementation(AActor* Attacker, float Damage) override;

	virtual void StartWeaponTrace_Implementation() override;
	virtual void StopWeaponTrace_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Attack();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UAttributesComponent* AttributesComponent;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	USoundBase* HitSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	EPawnState CurrentState;


	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* DeathMontage;


	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Combat")
	APickableWeapon* EquippedWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<APickableWeapon> DefaultWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	FName WeaponSocketName = TEXT("WeaponSocket");

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void HandleDeath();

	void PerformAttackTrace();

	bool bIsAttacking = false;

	UPROPERTY()
	TArray<AActor*> HitActors;
};