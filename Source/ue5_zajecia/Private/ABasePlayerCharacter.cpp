#include "ABasePlayerCharacter.h"
#include "Widget/MainHUD.h"
#include "ABaseEnemyCharacter.h"
#include "ABasePlayerController.h"
#include "InteractionComponent.h"
#include "InputMappingContext.h"
#include "PickableWeapon.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputActionValue.h"
#include "Components/SkeletalMeshComponent.h"
#include "CombatInterface.h"
#include "AttributesComponent.h"

#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/HitResult.h"
#include "Engine/EngineTypes.h"
#include "Math/Quat.h"
#include "Math/Color.h"

AABasePlayerCharacter::AABasePlayerCharacter()
{
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
	AttributesComponent = CreateDefaultSubobject<UAttributesComponent>(TEXT("AttributesComponent"));
	PrimaryActorTick.bCanEverTick = true;
	bIsAttacking = false;
}


void AABasePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			
			if (MappingContext)
			{
				Subsystem->AddMappingContext(MappingContext, 0);
			}
		}
	}
}

void AABasePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		
		if (MoveAction)
		{
			
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AABasePlayerCharacter::Move);
		}

		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AABasePlayerCharacter::Look);
		}
		if (EquipAction)
		{
			EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Started, this, &AABasePlayerCharacter::Interact);
		}
		if (AttackAction)
		{
			EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AABasePlayerCharacter::Attack);
		}
	}

	
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* LP = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				if (MappingContext)
				{
					Subsystem->AddMappingContext(MappingContext, 0);
				}
			}
		}
	}
}

void AABasePlayerCharacter::Move(const FInputActionValue& Value)
{

	if (CurrentState == EPawnState::EPS_Occupied ||
		CurrentState == EPawnState::EPS_Hit ||
		CurrentState == EPawnState::EPS_Dead)
	{
		return;
	}
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FVector ForwardDirection = GetActorForwardVector();
		const FVector RightDirection = GetActorRightVector();

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AABasePlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookVector.X);
		AddControllerPitchInput(-LookVector.Y);
	}
}

void AABasePlayerCharacter::Equip(APickableWeapon* Weapon)
{
	if (!Weapon) return;

	CurrentWeapon = Weapon;

	FName SocketName = TEXT("WeaponSocket");

	if (Weapon->WeaponMesh)
	{
		Weapon->WeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
		UE_LOG(LogTemp, Warning, TEXT("WeaponMesh po podczepieniu: %s"), *Weapon->WeaponMesh->GetComponentLocation().ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Brak WeaponMesh!"));
	}

	if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(Weapon->GetRootComponent()))
	{
		PrimComp->SetSimulatePhysics(false);
		PrimComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}


}
void AABasePlayerCharacter::Interact()
{
	if (InteractionComponent)
	{
		InteractionComponent->TryInteract(this);
	}
}

void AABasePlayerCharacter::Attack(const FInputActionValue& Value)
{
	if (CurrentState == EPawnState::EPS_Exhausted || CurrentState == EPawnState::EPS_Occupied)
	{
		return;
	}
	if (!CurrentWeapon)
	{
		return;
	}

	if (AttributesComponent)
	{
		float AttackCost = AttributesComponent->StaminaCosts.StaminaCost_Attack;

		if (!AttributesComponent->CanPayStaminaCost(AttackCost))
		{
			return;
		}
	}

	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastAttackTime < AttackCooldown)
	{
		return;
	}

	if (GetMesh()->GetAnimInstance() && GetMesh()->GetAnimInstance()->Montage_IsPlaying(AttackMontage))
	{
		return;
	}

	if (AttributesComponent)
	{
		float AttackCost = AttributesComponent->StaminaCosts.StaminaCost_Attack;
		AttributesComponent->PayStaminaCost(AttackCost);
	}

	LastAttackTime = CurrentTime;

	UE_LOG(LogTemp, Warning, TEXT("Attack triggered!"));
	if (AttackMontage)
	{
		SetCurrentState(EPawnState::EPS_Occupied);
		PlayAnimMontage(AttackMontage);

		float MontageDuration = AttackMontage->GetPlayLength();
		GetWorld()->GetTimerManager().SetTimer(
			AttackTimerHandle,
			this,
			&AABasePlayerCharacter::FinishAttack,
			MontageDuration,
			false
		);
	}
}

void AABasePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsAttacking)
	{
		PerformAttackTrace();
	}

	if (AttributesComponent)
	{
		float CurrentStamina = AttributesComponent->GetStamina();

		if (CurrentStamina <= 0.0f && CurrentState != EPawnState::EPS_Exhausted)
		{
			SetCurrentState(EPawnState::EPS_Exhausted);

			GetCharacterMovement()->MaxWalkSpeed = 300.0f;
			UE_LOG(LogTemp, Warning, TEXT("Jestem wyczerpany!"));
		}

		if (CurrentState == EPawnState::EPS_Exhausted)
		{
			if (CurrentStamina >= StaminaThreshold)
			{
				SetCurrentState(EPawnState::EPS_Idle);

				GetCharacterMovement()->MaxWalkSpeed = 600.0f;
				UE_LOG(LogTemp, Warning, TEXT("Odzyskalem sily!"));
			}
		}
	}
}


void AABasePlayerCharacter::StartWeaponTrace()
{
	bIsAttacking = true;
	HitActors.Empty(); 
}

void AABasePlayerCharacter::EndWeaponTrace()
{
	bIsAttacking = false;
}


void AABasePlayerCharacter::PerformAttackTrace()
{
	if (!CurrentWeapon || !CurrentWeapon->GetHitbox())
	{
		return;
	}

	UBoxComponent* Hitbox = CurrentWeapon->GetHitbox();

	FVector Start = Hitbox->GetComponentLocation();
	FVector End = Start;
	FVector HalfSize = Hitbox->GetScaledBoxExtent();
	FRotator Orientation = Hitbox->GetComponentRotation();
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(CurrentWeapon);
	FHitResult HitResult;

	bool bHit = UKismetSystemLibrary::BoxTraceSingle(
		GetWorld(),
		Start,
		End,
		HalfSize,
		Orientation,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		0.1f
	);

	if (bHit)
	{
		// Debug box (opcjonalnie)
		UKismetSystemLibrary::DrawDebugBox(
			GetWorld(), HitResult.Location, HalfSize, FLinearColor::Green, Orientation, 0.1f, 1.5f
		);

		AActor* HitActor = HitResult.GetActor();

		if (!HitActors.Contains(HitActor))
		{
			HitActors.Add(HitActor);

			FVector HitLocation = HitResult.Location;
			UE_LOG(LogTemp, Warning, TEXT("Trafiono %s w miejscu: %s"), *HitActor->GetName(), *HitLocation.ToString());

			if (HitActor->Implements<UCombatInterface>())
			{
				float DamageToDeal = CurrentWeapon->BaseDamage;
				ICombatInterface::Execute_GetHit(HitActor, this, DamageToDeal);

				// === NOWA CZÊŒÆ: AKTUALIZACJA HUD (Zadanie 5) ===

				// 1. Rzutujemy trafionego aktora na Wroga
				if (AABaseEnemyCharacter* Enemy = Cast<AABaseEnemyCharacter>(HitActor))
				{
					// 2. Pobieramy jego komponent atrybutów 
					// (Jeœli ta linijka œwieci na czerwono, upewnij siê, ¿e AttributesComponent 
					// jest w sekcji 'public' w ABaseEnemyCharacter.h lub u¿yj gettera)
					if (UAttributesComponent* EnemyAttributes = Enemy->AttributesComponent)
					{
						// 3. Pobieramy nasz Kontroler
						if (AABasePlayerController* PC = Cast<AABasePlayerController>(GetController()))
						{
							// 4. Pobieramy HUD i wysy³amy dane
							if (UMainHUD* HUD = Cast<UMainHUD>(PC->HUDInstance))
							{
								HUD->UpdateEnemyHealth(EnemyAttributes->GetHealth(), EnemyAttributes->GetMaxHealth());
							}
						}
					}
				}
				// ===============================================
			}
		}
	}
}

void AABasePlayerCharacter::SetCurrentState(EPawnState NewState)
{
	if (CurrentState != NewState)
	{
		CurrentState = NewState;
		OnStateChanged.Broadcast(CurrentState);
	}
}

void AABasePlayerCharacter::FinishAttack()
{
	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);

	SetCurrentState(EPawnState::EPS_Idle);

	HitActors.Empty();
}