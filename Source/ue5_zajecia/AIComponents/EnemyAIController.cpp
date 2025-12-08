#include "AIComponents/EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ABaseEnemyCharacter.h"
#include "ABasePlayerCharacter.h"
#include "Perception/AIPerceptionComponent.h"


AEnemyAIController::AEnemyAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTree)
	{
		UBlackboardComponent* BlackboardComp;
		if (UseBlackboard(BehaviorTree->BlackboardAsset, BlackboardComp))
		{
			Blackboard = BlackboardComp;
			RunBehaviorTree(BehaviorTree);
		}
	}

	if (AIPerceptionComponent)
	{
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetPerceptionUpdated);
	}
}

void AEnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	AABasePlayerCharacter* Player = Cast<AABasePlayerCharacter>(Actor);
	if (!Player || !Blackboard) return;

	if (Stimulus.WasSuccessfullySensed())
	{
		Blackboard->SetValueAsObject(TEXT("TargetActor"), Player);
	}
	else
	{
		if (Blackboard->GetValueAsObject(TEXT("TargetActor")) == Player)
		{
			Blackboard->SetValueAsVector(TEXT("LastKnownPlayerLocation"), Player->GetActorLocation());

			Blackboard->ClearValue(TEXT("TargetActor"));
		}
	}
}

void AEnemyAIController::OnOwnerDeath()
{
	if (Blackboard)
	{
		Blackboard->SetValueAsBool(TEXT("IsDead"), true);
	}
	BrainComponent->StopLogic(TEXT("Owner Died"));
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
}