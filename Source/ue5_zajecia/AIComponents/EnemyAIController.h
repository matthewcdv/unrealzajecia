// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyAIController.generated.h"


class UBehaviorTree;
class UBlackboardComponent;
class UAIPerceptionComponent;
/**
 * 
 */
UCLASS()
class UE5_ZAJECIA_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override; // U¿ywany do uruchomienia BT

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTree; // Referencja do assetu BT

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionComponent* AIPerceptionComponent;

	// Funkcja wywo³ywana przez AIPerceptionComponent (Zadanie 5, p. 2)
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	// Funkcja do obs³ugi œmierci (Zadanie 5, p. 135)
	UFUNCTION()
	void OnOwnerDeath();

private:
	// Pomocnicza zmienna dla Perception
	AActor* LastKnownTarget = nullptr;
};
