#include "BTTask_Attack.h"
#include "AIController.h"
#include "ABaseEnemyCharacter.h" // Pamiêtaj o poprawnej œcie¿ce!

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = "Attack Player"; // Tak siê bêdzie nazywa³ w Edytorze
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
		// Pobierz wroga, który jest kontrolowany
		AABaseEnemyCharacter* Enemy = Cast<AABaseEnemyCharacter>(AIController->GetPawn());
		if (Enemy)
		{
			// Wywo³aj funkcjê ataku, któr¹ zrobiliœmy wczeœniej
			Enemy->Attack();
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}