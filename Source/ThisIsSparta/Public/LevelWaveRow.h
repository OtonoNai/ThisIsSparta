#pragma once

#include "CoreMinimal.h"
#include "LevelWaveRow.generated.h"

USTRUCT(BlueprintType)
struct THISISSPARTA_API FLevelWaveRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 WaveTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 SpawnItemCount;
};
