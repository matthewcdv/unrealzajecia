#include "ABaseEnemyCharacter.h"
#include "AttributesComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

AABaseEnemyCharacter::AABaseEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	AttributesComponent = CreateDefaultSubobject<UAttributesComponent>(TEXT("AttributesComponent"));
	CurrentState = EPawnState::EPS_Idle;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->SightRadius = 1500.0f;
	PawnSensingComp->SetPeripheralVisionAngle(60.0f);
}

void AABaseEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AttributesComponent)
	{
		AttributesComponent->OnDeath.AddDynamic(this, &AABaseEnemyCharacter::HandleDeath);
	}

	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AABaseEnemyCharacter::OnSeePawn);
	}
}

void AABaseEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentState == EPawnState::EPS_Dead || CurrentState == EPawnState::EPS_Hit) return;

	if (CombatTarget && CurrentState != EPawnState::EPS_Occupied)
	{
		float Distance = GetDistanceTo(CombatTarget);

		if (Distance <= AttackRange)
		{
			FVector Direction = CombatTarget->GetActorLocation() - GetActorLocation();
			Direction.Z = 0.0f;
			FRotator LookAtRotation = Direction.Rotation();

			SetActorRotation(LookAtRotation);
			PerformAttack();
		}
	}
}

void AABaseEnemyCharacter::OnSeePawn(APawn* Pawn)
{
	if (Pawn && Pawn != CombatTarget)
	{
		CombatTarget = Pawn;
	}
}

void AABaseEnemyCharacter::PerformAttack()
{
	if (CurrentState == EPawnState::EPS_Occupied) return;

	CurrentState = EPawnState::EPS_Occupied;

	if (AttackMontage)
	{
		PlayAnimMontage(AttackMontage);
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AABaseEnemyCharacter::OnAttackMontageEnded);
		GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate, AttackMontage);
	}
}

void AABaseEnemyCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	CurrentState = EPawnState::EPS_InCombat;
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
		if (CurrentState == EPawnState::EPS_Occupied)
		{
			StopAnimMontage();
		}

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

	SetActorTickEnabled(false);

	if (PawnSensingComp)
	{
		PawnSensingComp->Deactivate();
	}
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetLifeSpan(5.0f);
}