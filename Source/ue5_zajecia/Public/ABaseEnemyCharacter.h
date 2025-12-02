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

	// Interfejs Combat
	virtual void GetHit_Implementation(AActor* Attacker, float Damage) override;

	// === PRZYWRACAMY TE FUNKCJE (Dla AnimNotify) ===
	virtual void StartWeaponTrace_Implementation() override;
	virtual void StopWeaponTrace_Implementation() override;

	// === FUNKCJA ATAKU (PUBLICZNA DLA AI) ===
	// Zmieniliœmy nazwê na Attack, ¿eby pasowa³a do BTTask, i przenieœliœmy do public
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Attack();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UAttributesComponent* AttributesComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	USoundBase* HitSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	EPawnState CurrentState;

	// USUNIÊTO: UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* AttackMontage;

	// USUNIÊTO: AActor* CombatTarget; (To teraz bêdzie w Blackboardzie)

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Combat")
	APickableWeapon* EquippedWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<APickableWeapon> DefaultWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	FName WeaponSocketName = TEXT("WeaponSocket");

	// USUNIÊTO: float AttackRange; (To teraz bêdzie w Behavior Tree)

	// USUNIÊTO: void OnSeePawn(APawn* Pawn);

	// Funkcje pomocnicze
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void HandleDeath();

	// === PRZYWRACAMY LOGIKÊ UDERZENIA ===
	void PerformAttackTrace();

	bool bIsAttacking = false;

	UPROPERTY()
	TArray<AActor*> HitActors;
};