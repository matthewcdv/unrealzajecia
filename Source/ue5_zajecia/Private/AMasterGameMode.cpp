// Fill out your copyright notice in the Description page of Project Settings.


#include "AMasterGameMode.h"
#include "ABasePlayerController.h"
#include "ABasePlayerCharacter.h"

AAMasterGameMode::AAMasterGameMode()
{
	DefaultPawnClass = AABasePlayerCharacter::StaticClass();
	PlayerControllerClass = AABasePlayerController::StaticClass();
}

