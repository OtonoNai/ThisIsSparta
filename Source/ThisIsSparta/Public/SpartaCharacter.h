#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpartaCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class THISISSPARTA_API ASpartaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASpartaCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<UCameraComponent> CameraComp;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Move(const FInputActionValue& IAValue);
	UFUNCTION()
	void StartJump(const FInputActionValue& IAValue);
	UFUNCTION()
	void EndJump(const FInputActionValue& IAValue);
	UFUNCTION()
	void Look(const FInputActionValue& IAValue);
	UFUNCTION()
	void StartSprint(const FInputActionValue& IAValue);
	UFUNCTION()
	void EndSprint(const FInputActionValue& IAValue);

private:
	float NormalSpeed;
	float SprintSpeedMultiplier;
	float SprintSpeed;
};
