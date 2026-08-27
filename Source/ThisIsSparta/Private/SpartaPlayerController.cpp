#include "SpartaPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "SpartaGameInstance.h"
#include "SpartaGameState.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

ASpartaPlayerController::ASpartaPlayerController()
	: InputMappingContext(nullptr),
	  MoveAction(nullptr),
	  JumpAction(nullptr),
	  LookAction(nullptr),
	  SprintAction(nullptr)
{
}

UUserWidget* ASpartaPlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

void ASpartaPlayerController::ShowGameHUD()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;

	}
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();

			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());
		}

		Cast<ASpartaGameState>(GetWorld()->GetGameState<ASpartaGameState>())->UpdateHUDWidget();
	}
}

void ASpartaPlayerController::ShowMainMenu(bool bIsRestart)
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();

			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}

		if (UTextBlock* ButtonText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName("Start")))
		{
			bIsRestart ?
			ButtonText->SetText(FText::FromString(TEXT("Restart")))
			: ButtonText->SetText(FText::FromString(TEXT("Start")));
		}
		
		if (bIsRestart)
		{
			UFunction* PlayAnimFunction = MainMenuWidgetInstance->FindFunction(FName(TEXT("PlayGameOverAnim")));
			if (PlayAnimFunction)
			{
				MainMenuWidgetInstance->ProcessEvent(PlayAnimFunction, nullptr);
			}
			
			if (UTextBlock* TotalScoreText =
				Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("TotalScore"))))
			{
				if (USpartaGameInstance* SpartaGameInstance =
					Cast<USpartaGameInstance>(UGameplayStatics::GetGameInstance(this)))
				{
					TotalScoreText->SetText(
						FText::Format(INVTEXT("Score : {0}"), SpartaGameInstance->GetTotalScore()));
				}
			}
			
		}
	}
}

void ASpartaPlayerController::StartGame()
{
	if (USpartaGameInstance* SpartaGameInstance = GetWorld()->GetGameInstance<USpartaGameInstance>())
	{
		SpartaGameInstance->InitializeGameData();
	}

	if (ASpartaGameState* SpartaGameState = GetWorld()->GetGameState<ASpartaGameState>())
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, SpartaGameState->GetFirstLevel());
	}
	
	SetPause(false);
}

void ASpartaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains(TEXT("MenuLevel")))
	{
		ShowMainMenu(false);
	}
}
