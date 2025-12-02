#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"


UINTERFACE(MinimalAPI)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

class UE5_ZAJECIA_API ICombatInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void GetHit(AActor* Attacker, float Damage);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void StartWeaponTrace();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void StopWeaponTrace();
};
