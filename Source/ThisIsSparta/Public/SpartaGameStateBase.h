#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SpartaGameStateBase.generated.h"

UCLASS()
class THISISSPARTA_API ASpartaGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ASpartaGameStateBase();
	
	UFUNCTION(BlueprintCallable, Category = "Score")
	int32 GetScore() const;
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 Score;
};
