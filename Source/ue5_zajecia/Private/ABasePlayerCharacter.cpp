// W pliku ABasePlayerCharacter.cpp

#include "ABasePlayerCharacter.h"
#include "Components/InputComponent.h"                  // Potrzebne do SetupPlayerInputComponent
#include "EnhancedInputComponent.h"                 // Potrzebne do bindowania Enhanced Input
#include "EnhancedInputSubsystems.h"                // Potrzebne do pobrania Subsystemu
#include "GameFramework/PlayerController.h"         // Potrzebne do pobrania PlayerController

// --- DODAJ TÊ FUNKCJÊ (REJESTRACJA KONTEKSTU) ---
void AABasePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Upewnij siê, ¿e mamy valid PlayerController
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		// Pobierz lokalny subsystem Enhanced Input
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Dodaj nasz Mapping Context. Upewnij siê, ¿e zmienna MappingContext nie jest pusta (sprawdzamy, czy przypisa³eœ j¹ w Blueprincie w Kroku 2)
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
		// Zbinduj (po³¹cz) akcjê. 
		// SprawdŸ czy MoveAction nie jest puste (czy przypisa³eœ w Blueprincie)
		if (MoveAction)
		{
			// Gdy akcja jest "Triggered" (czyli klawisz jest wciœniêty), wywo³aj funkcjê "Move" w tym obiekcie ("this")
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AABasePlayerCharacter::Move);
		}

		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AABasePlayerCharacter::Look);
		}
	}
}

// --- DODAJ TÊ FUNKCJÊ (LOGIKA RUCHU) ---
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