#include "SpartaGameMode.h"
#include "SpartaCharacter.h"
#include "SpartaGameStateBase.h"
#include "SpartaPlayerController.h"

ASpartaGameMode::ASpartaGameMode()
{
	DefaultPawnClass = ASpartaCharacter::StaticClass();
	PlayerControllerClass = ASpartaPlayerController::StaticClass();
	GameStateClass = ASpartaGameStateBase::StaticClass();
}
