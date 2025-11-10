// Fill out your copyright notice in the Description page of Project Settings.


#include "StartNotifyTrace.h"
#include "ABasePlayerCharacter.h"

void UStartNotifyTrace::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::Notify(MeshComp, Animation, EventReference);

    UE_LOG(LogTemp, Warning, TEXT("--- StartTraceNotify Start notifaja w animacji ---"));
    if (AActor* Owner = MeshComp->GetOwner())
    {
        if (AABasePlayerCharacter* Character = Cast<AABasePlayerCharacter>(Owner))
        {
            Character->StartWeaponTrace();
        }
    }
}