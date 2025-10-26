// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"
#include "InteractionInterface.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInteractionComponent::TryInteract(AActor* Interactor)
{
    FVector Start = Interactor->GetActorLocation();
    FVector Forward = Interactor->GetActorForwardVector();
    FVector End = Start + Forward * 200.f;

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(Interactor);

    float SphereRadius = 150.f;
    if (GetWorld()->SweepSingleByChannel(Hit, Start, End, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(SphereRadius), Params))
    {
        AActor* HitActor = Hit.GetActor();
        if (HitActor && HitActor->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
        {
            IInteractionInterface* Interaction = Cast<IInteractionInterface>(HitActor);
            if (Interaction)
            {
                Interaction->Interact(Interactor);
            }
        }
    }
}
