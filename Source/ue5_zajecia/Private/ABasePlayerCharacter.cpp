// W pliku ABasePlayerCharacter.cpp

#include "ABasePlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputActionValue.h"
#include "InteractionComponent.h" 
#include "PickableWeapon.h"
#include "Components/SkeletalMeshComponent.h"

#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/HitResult.h"
#include "Engine/EngineTypes.h"
#include "Math/Quat.h"
#include "Math/Color.h"

AABasePlayerCharacter::AABasePlayerCharacter()
{
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
	PrimaryActorTick.bCanEverTick = true;
	bIsAttacking = false;
}


void AABasePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Upewnij siê, ¿e mamy valid PlayerController
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		// Pobierz lokalny subsystem Enhanced Input
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			
			if (MappingContext)
			{
				Subsystem->AddMappingContext(MappingContext, 0);
			}
		}
	}
}

// --- DODAJ TÊ FUNKCJÊ (BINDOWANIE AKCJI) ---
void AABasePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Rzutuj InputComponent na EnhancedInputComponent
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
			EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &AABasePlayerCharacter::Interact);
		}
		if (AttackAction)
		{
			EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AABasePlayerCharacter::Attack);
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

// --- (LOGIKA RUCHU) ---
void AABasePlayerCharacter::Move(const FInputActionValue& Value)
{
	// Pobierz wartoœæ z akcji jako Wektor 2D (dziêki Krok 1)
	const FVector2D MovementVector = Value.Get<FVector2D>();

	// Upewnij siê, ¿e postaæ ma kontroler
	if (Controller != nullptr)
	{
		// Pobierz wektor "do przodu" postaci
		const FVector ForwardDirection = GetActorForwardVector();
		// Pobierz wektor "w prawo" postaci
		const FVector RightDirection = GetActorRightVector();

		// Dodaj ruch do przodu/ty³u (oœ Y z wektora 2D)
		AddMovementInput(ForwardDirection, MovementVector.Y);
		// Dodaj ruch w lewo/prawo (oœ X z wektora 2D)
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AABasePlayerCharacter::Look(const FInputActionValue& Value)
{
	// Pobierz wartoœæ z akcji jako Wektor 2D
	const FVector2D LookVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Dodaj obrót "Yaw" (lewo/prawo) - Oœ X myszki
		AddControllerYawInput(LookVector.X);

		// Dodaj obrót "Pitch" (góra/dó³) - Oœ Y myszki
		AddControllerPitchInput(-LookVector.Y);
	}
}

void AABasePlayerCharacter::Equip(APickableWeapon* Weapon)
{
	UE_LOG(LogTemp, Warning, TEXT("Equip wywo³ane!"));
	if (!Weapon) return;

	CurrentWeapon = Weapon;

	FName SocketName = TEXT("WeaponSocket");

	// Podczepiamy bezpoœrednio WeaponMesh do mesh'a postaci
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
	UE_LOG(LogTemp, Warning, TEXT("Attack triggered!"));
	if (AttackMontage)
	{
		PlayAnimMontage(AttackMontage);
	}
}

void AABasePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsAttacking)
	{
		PerformAttackTrace();
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
		return; // Nie mamy broni albo broñ nie ma hitboxa
	}

	UBoxComponent* Hitbox = CurrentWeapon->GetHitbox();

	FVector Start = Hitbox->GetComponentLocation();
	FVector End = Start; // Skanujemy w miejscu, wiêc Start i End s¹ te same
	FVector HalfSize = Hitbox->GetScaledBoxExtent();
	FRotator Orientation = Hitbox->GetComponentRotation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this); // Ignoruj sam¹ postaæ
	ActorsToIgnore.Add(CurrentWeapon); // Ignoruj sam¹ broñ

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
		EDrawDebugTrace::ForDuration,
		HitResult,
		true,
		FLinearColor::Red,     // Kolor gdy nie trafia
		FLinearColor::Green,   // Kolor gdy trafia
		0.1f                   // Czas rysowania
	);

	if (bHit)
	{
		// Sprawdzamy, czy ju¿ nie trafiliœmy tego aktora w tym machniêciu
		if (!HitActors.Contains(HitResult.GetActor()))
		{
			HitActors.Add(HitResult.GetActor()); // Dodaj do listy trafionych

			// ZDERZENIE!
			FVector HitLocation = HitResult.Location;
			AActor* HitActor = HitResult.GetActor();

			UE_LOG(LogTemp, Warning, TEXT("Trafiono %s w miejscu: %s"), *HitActor->GetName(), *HitLocation.ToString());

			// Tutaj mo¿esz dodaæ logikê zadawania obra¿eñ, np.
			// UGameplayStatics::ApplyDamage(HitActor, 10.f, GetController(), this, UDamageType::StaticClass());
		}
	}
}