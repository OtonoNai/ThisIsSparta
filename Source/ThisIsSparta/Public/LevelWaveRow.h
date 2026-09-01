#pragma once

#include "CoreMinimal.h"
#include "LevelWaveRow.generated.h"

USTRUCT(BlueprintType)
struct THISISSPARTA_API FLevelWaveRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 WaveTime = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 SpawnItemCount = 0;
};
