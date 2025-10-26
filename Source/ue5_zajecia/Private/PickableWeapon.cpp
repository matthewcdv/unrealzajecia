// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableWeapon.h"
#include "ABasePlayerCharacter.h" 

APickableWeapon::APickableWeapon()
{
    PrimaryActorTick.bCanEverTick = false;
}

void APickableWeapon::PickUp(AActor* Interactor)
{
    AABasePlayerCharacter* Player = Cast<AABasePlayerCharacter>(Interactor);
    if (Player)
    {
        Player->Equip(this);
    }
}

