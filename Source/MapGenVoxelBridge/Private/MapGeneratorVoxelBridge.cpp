// Fill out your copyright notice in the Description page of Project Settings.

#include "MapGenVoxelBridgePrivatePCH.h"
#include "Maps/Biomes/WhittakerBiomeManager.h"
#include "MapGeneratorVoxelBridge.h"


void UMapGeneratorVoxelBridge::CreateVoxelsFromMap(AIslandMapGenerator* MapGenerator, APagedVolume* VoxelVolume, int32 MapHeight)
{
	if (MapGenerator == NULL || VoxelVolume == NULL)
	{
		UE_LOG(LogTemp, Error, TEXT("Map Generator or Voxel Volume was invalid!"));
		return;
	}
	TArray<float> heightmapFloats;
	TArray<uint8> materials;

	// Fetch map heightmap data
	TArray<FMapData> mapData = MapGenerator->GetHeightmap()->GetMapData();

	heightmapFloats.SetNumZeroed(mapData.Num());
	materials.SetNumZeroed(mapData.Num());

	for (int i = 0; i < mapData.Num(); i++)
	{
		heightmapFloats[i] = mapData[i].Elevation;
		materials[i] = (uint8)UWhittakerBiomeManager::ConvertToWhittakerBiomeEnum(mapData[i].Biome);
	}

	FIslandData islandData = MapGenerator->IslandData;
	FRegion mapRegion;
	mapRegion.UpperX = islandData.Size;
	mapRegion.UpperY = islandData.Size;
	mapRegion.UpperZ = MapHeight;

	UPagedVolumeComponent* pagedVolume = VoxelVolume->GetPagedVolume();
	pagedVolume->SetRegionVoxels(mapRegion, heightmapFloats, materials);
}
