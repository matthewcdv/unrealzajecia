// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ABaseCharacter.h"
#include "InteractionComponent.h"
#include "InputMappingContext.h"
#include "PickableWeapon.h"
#include "ABasePlayerCharacter.generated.h"

/**
 * 
 */
class InputMappingContext;
class UInputAction;
UCLASS()
class UE5_ZAJECIA_API AABasePlayerCharacter : public AABaseCharacter
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
    UInputMappingContext* MappingContext;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
    UInputAction* MoveAction;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
    UInputAction* EquipAction;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
    UInputAction* AttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAction;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	APickableWeapon* CurrentWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	UInteractionComponent* InteractionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	UAnimMontage* AttackMontage;
protected:

	virtual void BeginPlay() override;


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void Interact();
public:
	virtual void Equip(APickableWeapon* Weapon);
	AABasePlayerCharacter();

	void Attack(const FInputActionValue& Value);

	void StartWeaponTrace();
	void EndWeaponTrace();

	virtual void Tick(float DeltaTime) override;

protected:
	void PerformAttackTrace();


private:
	bool bIsAttacking;

	UPROPERTY()
	TArray<AActor*> HitActors;

	float AttackCooldown = 0.5f; // czas w sekundach
	float LastAttackTime = -100.f;
};
