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
protected:
	// --- DODAJ TE FUNKCJE ---

	/** Wywo³ywane na pocz¹tku gry */
	virtual void BeginPlay() override;

	/** Wywo³ywane, by przypisaæ input do funkcji */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Funkcja, która bêdzie wywo³ywana przez IA_Move */
	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void Attack();

	void Interact();
public:
	virtual void Equip(APickableWeapon* Weapon);
	AABasePlayerCharacter();
};
