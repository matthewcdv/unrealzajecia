#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "InteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5_ZAJECIA_API UInteractionComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UInteractionComponent();
protected:
	virtual void BeginPlay() override;
public:
	void TryInteract(AActor* Interactor);
};
