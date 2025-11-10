// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableWeapon.h"
#include "ABasePlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

APickableWeapon::APickableWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    
    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    SetRootComponent(WeaponMesh);

    
    HitboxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("HitboxComponent"));
    HitboxComponent->SetupAttachment(WeaponMesh);

    
    HitboxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    HitboxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);

    
    GripPoint = CreateDefaultSubobject<USceneComponent>(TEXT("GripPoint"));
    GripPoint->SetupAttachment(WeaponMesh);
}

void APickableWeapon::PickUp(AActor* Interactor)
{
    UE_LOG(LogTemp, Warning, TEXT("PickUp wywo³ane!"));
    AABasePlayerCharacter* Player = Cast<AABasePlayerCharacter>(Interactor);
    if (Player)
    {
        Player->Equip(this);
    }
}

