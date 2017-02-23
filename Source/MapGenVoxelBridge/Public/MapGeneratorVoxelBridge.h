// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Maps/IslandMapGenerator.h"
#include "Paging/LandWaterVolume.h"
#include "MapGeneratorVoxelBridge.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class MAPGENVOXELBRIDGE_API UMapGeneratorVoxelBridge : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="Voxel|Map")
	static void CreateVoxelsFromMap(AIslandMapGenerator* MapGenerator, ALandWaterVolume* VoxelVolume, int32 MapHeight);
};