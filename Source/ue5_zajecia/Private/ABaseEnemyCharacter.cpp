#include "ABaseEnemyCharacter.h"
#include "PickableWeapon.h"
#include "AttributesComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h" // Potrzebne do BoxTrace
#include "Components/BoxComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

AABaseEnemyCharacter::AABaseEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	AttributesComponent = CreateDefaultSubobject<UAttributesComponent>(TEXT("AttributesComponent"));
	CurrentState = EPawnState::EPS_Idle;

	// USUNIÊTO: PawnSensingComp = ... (AIController teraz ma AIPerception)
}

void AABaseEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AttributesComponent)
	{
		AttributesComponent->OnDeath.AddDynamic(this, &AABaseEnemyCharacter::HandleDeath);
	}

	// USUNIÊTO: PawnSensingComp->OnSeePawn...

	if (DefaultWeaponClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		APickableWeapon* SpawnedWeapon = GetWorld()->SpawnActor<APickableWeapon>(DefaultWeaponClass, GetActorLocation(), GetActorRotation(), SpawnParams);

		if (SpawnedWeapon)
		{
			EquippedWeapon = SpawnedWeapon;
			EquippedWeapon->GetRootComponent()->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocketName);
			EquippedWeapon->SetActorEnableCollision(false);
		}
	}
}

void AABaseEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// USUNIÊTO: Ca³¹ logikê chodzenia do gracza i atakowania.
	// Teraz robi to Behavior Tree!

	// Ale musimy zostawiæ logikê BoxTrace (¿eby ciosy rani³y)
	if (bIsAttacking)
	{
		PerformAttackTrace();
	}
}

// Funkcja wywo³ywana przez BTTask_Attack
void AABaseEnemyCharacter::Attack()
{
	if (CurrentState == EPawnState::EPS_Occupied || CurrentState == EPawnState::EPS_Dead) return;

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
	// Po ataku wracamy do InCombat (¿eby AI mog³o dalej decydowaæ)
	CurrentState = EPawnState::EPS_InCombat;
}

// === PRZYWRÓCONE FUNKCJE DO ZADAWANIA OBRA¯EÑ ===

void AABaseEnemyCharacter::StartWeaponTrace_Implementation()
{
	bIsAttacking = true;
	HitActors.Empty();
}

void AABaseEnemyCharacter::StopWeaponTrace_Implementation()
{
	bIsAttacking = false;
}

void AABaseEnemyCharacter::PerformAttackTrace()
{
	if (!EquippedWeapon || !EquippedWeapon->GetHitbox()) return;

	FVector Start = EquippedWeapon->GetHitbox()->GetComponentLocation();
	FVector End = Start;
	FVector HalfSize = EquippedWeapon->GetHitbox()->GetScaledBoxExtent();
	FRotator Orientation = EquippedWeapon->GetHitbox()->GetComponentRotation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(EquippedWeapon);

	FHitResult HitResult;
	bool bHit = UKismetSystemLibrary::BoxTraceSingle(
		GetWorld(), Start, End, HalfSize, Orientation,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false, ActorsToIgnore, EDrawDebugTrace::ForDuration, // Debug w³¹czony
		HitResult, true, FLinearColor::Red, FLinearColor::Green, 0.1f
	);

	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor && !HitActors.Contains(HitActor))
		{
			HitActors.Add(HitActor);
			if (HitActor->Implements<UCombatInterface>())
			{
				ICombatInterface::Execute_GetHit(HitActor, this, EquippedWeapon->BaseDamage);
			}
		}
	}
}
// ===============================================

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

	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		AIController->StopMovement();

		if (UBlackboardComponent* BB = AIController->GetBlackboardComponent())
		{
			BB->SetValueAsBool(TEXT("IsDead"), true);
		}
	}

	//SetActorTickEnabled(false);
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();

	if (DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetLifeSpan(5.0f);
}