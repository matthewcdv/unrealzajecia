#include "ABaseEnemyCharacter.h"
#include "AttributesComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

AABaseEnemyCharacter::AABaseEnemyCharacter()
{
	AttributesComponent = CreateDefaultSubobject<UAttributesComponent>(TEXT("AttributesComponent"));
	CurrentState = EPawnState::EPS_Idle;
}

void AABaseEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AttributesComponent)
	{
		AttributesComponent->OnDeath.AddDynamic(this, &AABaseEnemyCharacter::HandleDeath);
	}
}

void AABaseEnemyCharacter::GetHit_Implementation(AActor* Attacker, float Damage)
{
	if (CurrentState == EPawnState::EPS_Dead || CurrentState == EPawnState::EPS_Hit)
	{
		return;
	}

	AttributesComponent->ApplyDamage(Damage);

	if (AttributesComponent->GetHealth() > 0.f)
	{
		CurrentState = EPawnState::EPS_Hit;

		if (HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
		}
		if (HitReactMontage)
		{
			PlayAnimMontage(HitReactMontage);
		}
	}
}

void AABaseEnemyCharacter::HandleDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("%s zginal!"), *GetName());
	CurrentState = EPawnState::EPS_Dead;

	GetCharacterMovement()->StopMovementImmediately();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
}