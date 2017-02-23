// Fill out your copyright notice in the Description page of Project Settings.

#include "MapGenVoxelBridgePrivatePCH.h"
#include "Maps/MapDataHelper.h"
#include "Maps/Biomes/WhittakerBiomeManager.h"
#include "MapGeneratorVoxelBridge.h"


void UMapGeneratorVoxelBridge::CreateVoxelsFromMap(AIslandMapGenerator* MapGenerator, ALandWaterVolume* VoxelVolume, int32 MapHeight)
{
	if (MapGenerator == NULL || VoxelVolume == NULL)
	{
		UE_LOG(LogTemp, Error, TEXT("Map Generator or Voxel Volume was invalid!"));
		return;
	}
	TArray<float> heightmapFloats;
	TArray<float> waterFloats;
	TArray<uint8> materials;
	TArray<uint8> liquidMaterials;

	// Fetch map heightmap data
	TArray<FMapData> mapData = MapGenerator->GetHeightmap()->GetMapData();

	heightmapFloats.SetNumZeroed(mapData.Num());
	waterFloats.SetNumZeroed(mapData.Num());
	materials.SetNumZeroed(mapData.Num());
	liquidMaterials.SetNumZeroed(mapData.Num());

	UWhittakerBiomeManager* biomeManager = (UWhittakerBiomeManager*)MapGenerator->BiomeManager;

	for (int i = 0; i < mapData.Num(); i++)
	{
		waterFloats[i] = 0.015f;
		heightmapFloats[i] = mapData[i].Elevation;
		if (UMapDataHelper::IsRiver(mapData[i]))
		{
			heightmapFloats[i] -= 0.15f;
			heightmapFloats[i] = FMath::Clamp(heightmapFloats[i], 0.0f, 1.0f);
			materials[i] = 0;
			waterFloats[i] = mapData[i].Elevation;
		}
		else if(biomeManager != NULL)
		{
			materials[i] = (uint8)biomeManager->ConvertToWhittakerBiomeEnum(mapData[i].Biome);
		}
	}

	FIslandData islandData = MapGenerator->IslandData;
	FRegion mapRegion;
	mapRegion.UpperX = islandData.Size;
	mapRegion.UpperY = islandData.Size;
	mapRegion.UpperZ = MapHeight;

	UPagedVolumeComponent* pagedVolume = VoxelVolume->GetPagedVolume();
	pagedVolume->SetRegionVoxels(mapRegion, heightmapFloats, materials);

	UPagedVolumeComponent* waterVolume = VoxelVolume->GetWaterPagedVolume();
	waterVolume->SetRegionVoxels(mapRegion, waterFloats, liquidMaterials);
}
