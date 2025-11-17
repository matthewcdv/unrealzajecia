#pragma once

#include "CoreMinimal.h"
#include "ABaseCharacter.h"
#include "CombatInterface.h"
#include "Enum/PawnState.h"
#include "Perception/PawnSensingComponent.h"
#include "ABaseEnemyCharacter.generated.h"


/**
 * 
 */
UCLASS()
class UE5_ZAJECIA_API AABaseEnemyCharacter : public AABaseCharacter, public ICombatInterface
{
	GENERATED_BODY()

public:
	AABaseEnemyCharacter();

	virtual void GetHit_Implementation(AActor* Attacker, float Damage) override;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* AttackMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere, Category = "AI")
	float AttackRange = 150.0f;

	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

	void PerformAttack();

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	void HandleDeath();


};
