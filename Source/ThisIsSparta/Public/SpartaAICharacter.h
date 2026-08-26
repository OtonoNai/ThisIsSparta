#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpartaAICharacter.generated.h"

UCLASS()
class THISISSPARTA_API ASpartaAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASpartaAICharacter();
	
	void SetMovementSpeed(float NewSpeed);

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float WalkSpeed;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float RunSpeed;
};
