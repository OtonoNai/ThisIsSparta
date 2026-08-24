#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SpartaGameState.generated.h"

UCLASS()
class THISISSPARTA_API ASpartaGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	ASpartaGameState();
	
	UFUNCTION(BlueprintCallable, Category = "Score")
	int32 GetScore() const;
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);
	
protected:
	virtual void BeginPlay() override;
	
private:
	int32 Score;
};
