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

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCameraComponent> CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = true))
	TObjectPtr<UWidgetComponent> OverheadHealth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = true))
	float MaxHealth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = true))
	float Health;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Speed", meta = (AllowPrivateAccess = true))
	float NormalSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Speed", meta = (AllowPrivateAccess = true))
	float SprintSpeedMultiplier;

	void UpdateOverheadHealth() const;
	void OnDeath();

	FNumberFormattingOptions HealthUIFormat;
};
