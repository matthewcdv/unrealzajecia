#pragma once

#include "CoreMinimal.h"

// UENUM pozwala u¿ywaæ tego enuma w Blueprintach
UENUM(BlueprintType)
enum class EPawnState : uint8
{
    EPS_Idle        UMETA(DisplayName = "Idle"),
    EPS_InCombat    UMETA(DisplayName = "In Combat"),
    EPS_Occupied    UMETA(DisplayName = "Occupied (Attacking)"),
    EPS_Hit         UMETA(DisplayName = "Hit Reaction"),
    EPS_Dead		UMETA(DisplayName = "Dead")
};