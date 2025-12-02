// Fill out your copyright notice in the Description page of Project Settings.


#include "AIComponents/EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ABaseEnemyCharacter.h"
#include "ABasePlayerCharacter.h"
#include "Perception/AIPerceptionComponent.h"


AEnemyAIController::AEnemyAIController()
{
	// Tworzymy komponent percepcji (alternatywa dla dodawania w klasie Pawn)
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTree)
	{
		UBlackboardComponent* BlackboardComp; // Zmienna tymczasowa
		if (UseBlackboard(BehaviorTree->BlackboardAsset, BlackboardComp))
		{
			Blackboard = BlackboardComp; // Przypisz wynik do g³ównej zmiennej
			RunBehaviorTree(BehaviorTree);
		}
	}

	// 3. Subskrybuj delegata percepcji
	if (AIPerceptionComponent)
	{
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetPerceptionUpdated);
	}
}

// Funkcja, która aktualizuje Blackboard, gdy AI "coœ" zobaczy
void AEnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	AABasePlayerCharacter* Player = Cast<AABasePlayerCharacter>(Actor);
	if (!Player || !Blackboard) return;

	if (Stimulus.WasSuccessfullySensed())
	{
		// === GRACZ ZAUWA¯ONY (Zadanie 5, p. 44) ===
		Blackboard->SetValueAsObject(TEXT("TargetActor"), Actor); // Ustaw cel
		// Tu mo¿esz dodaæ kod do zmiany PawnState na InCombat w Pawn
		// (póŸniej wywo³asz funkcjê w ABaseEnemyCharacter)
	}
	else if (Blackboard->GetValueAsObject(TEXT("TargetActor")) == Actor)
	{
		// === GRACZ ZGUBIONY (Zadanie 5, p. 45) ===
		LastKnownTarget = Actor; // Zapamiêtaj gracza

		// Zapisz ostatni¹ znan¹ pozycjê
		Blackboard->SetValueAsVector(TEXT("LastKnownPlayerLocation"), Actor->GetActorLocation());

		// Wyczyœæ cel
		Blackboard->ClearValue(TEXT("TargetActor"));
	}
}

void AEnemyAIController::OnOwnerDeath()
{
	// Wywo³ywane przez Enemy Pawn gdy umiera
	if (Blackboard)
	{
		// Ustaw flagê, by Behavior Tree siê zatrzyma³o
		Blackboard->SetValueAsBool(TEXT("IsDead"), true); // Zadanie 5, p. 114
	}
	// Zatrzymaj Behavior Tree
	BrainComponent->StopLogic(TEXT("Owner Died")); // Zadanie 5, p. 135
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
}