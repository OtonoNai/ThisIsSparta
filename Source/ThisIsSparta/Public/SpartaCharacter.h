#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpartaCharacter.generated.h"

class UWidgetComponent;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class THISISSPARTA_API ASpartaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASpartaCharacter();

	UFUNCTION(Category = "Health")
	float GetHealth() const;
	UFUNCTION(Category = "Health")
	void AddHealth(float Amount);

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(
		float DamageAmount,
		FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

	UFUNCTION()
	void Move(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void StartJump(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void EndJump(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void Look(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void StartSprint(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void EndSprint(const FInputActionValue& InputActionValue);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> OverheadHP;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	float MaxHealth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	float Health;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Speed")
	float NormalSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Speed")
	float SprintSpeedMultiplier;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Speed")
	float SprintSpeed;

private:
	void UpdateOverheadHP() const;
	void OnDeath();

	FNumberFormattingOptions HealthUIFormat;
};
