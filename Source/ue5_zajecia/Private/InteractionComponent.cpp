#include "InteractionComponent.h"
#include "InteractionInterface.h"
#include "DrawDebugHelpers.h"


UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
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
