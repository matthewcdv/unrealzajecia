// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseInteractionActor.h"
#include "PickableWeapon.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class UE5_ZAJECIA_API APickableWeapon : public ABaseInteractionActor
{
	GENERATED_BODY()
public:
    APickableWeapon();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* WeaponMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    UBoxComponent* HitboxComponent;

    virtual void PickUp(AActor* Interactor) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    USceneComponent* GripPoint;

    USceneComponent* GetGripPoint() const { return GripPoint; }

    UBoxComponent* GetHitbox() const { return HitboxComponent; }
	
};
