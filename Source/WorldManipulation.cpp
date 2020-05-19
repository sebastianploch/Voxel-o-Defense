#include "pch.h"
#include "WorldManipulation.h"
#include "ChunkHandler.h"
#include "PerlinNoise.h"

using namespace DirectX::SimpleMath;

const char WorldManipulation::GetVoxel(int x, int y, int z) {
	return GetVoxel(Vector3Int(x, y, z));;
}

const char WorldManipulation::GetVoxel(DirectX::SimpleMath::Vector3Int worldPos) {
	Chunk* c = ChunkHandler::GetChunk(worldPos);
	return c->GetVoxel(GetRelativePos(c, worldPos));
}

void WorldManipulation::SetVoxel(char v, int x, int y, int z) {
	SetVoxel(v, Vector3Int(x, y, z));
}

void WorldManipulation::SetVoxel(char v, Vector3Int worldPos) {
	Chunk* c = ChunkHandler::GetChunk(worldPos);
	return c->SetVoxel(v, GetRelativePos(c, worldPos));
}

void WorldManipulation::SetVoxelArea(char v, int x, int y, int z, int w, int h, int d) {
	SetVoxelArea(v, Vector3Int(x, y, z), Vector3Int(w, h, d));
}

void WorldManipulation::SetVoxelArea(char v, Vector3Int worldPos, Vector3Int size) {
	for (int x = 0; x < size.x; x++) {
		for (int y = 0; y < size.y; y++) {
			for (int z = 0; z < size.z; z++) {
				SetVoxel(v, Vector3Int(worldPos.x + x, worldPos.y + y, worldPos.z + z));
			}
		}
	}
}

void WorldManipulation::SetVoxelSphere(char v, int x, int y, int z, int r) {
	SetVoxelSphere(v, Vector3Int(x, y, z), r);
}

void WorldManipulation::SetVoxelSphere(char v, Vector3Int worldPos, int r) {
	for (int x = -r; x <= r; x++) {
		for (int y = -r; y <= r; y++) {
			for (int z = -r; z <= r; z++) {
				float dist = sqrt(pow(-x, 2) + pow(-y, 2) + pow(-z, 2)); //Distance from 0,0,0
				if(dist < r)
					SetVoxel(v, Vector3Int(worldPos.x + x, worldPos.y + y, worldPos.z + z));
			}
		}
	}
}

void WorldManipulation::GenerateTerrainData(Chunk* c) {
	for (int x = 0; x < c->GetWidth(); x++) {
		for (int z = 0; z < c->GetWidth(); z++) {

			Vector3Int voxPos = Vector3Int(x + (c->GetWidth() * c->GetXIndex()), 0, 
										   z + (c->GetDepth() * c->GetZIndex()));

			//Sample perlin noise for heightmap
			const int sampleCount = 5;
			float noiseSamples[sampleCount];						  //x frequency       z freq.     magnitude
			noiseSamples[0] = PerlinNoise::Noise(voxPos.x / 128.0f, voxPos.z / 128.0f);
			noiseSamples[1] = PerlinNoise::Noise(voxPos.x / 64.0f , voxPos.z / 64.0f)	/ 10.0f;
			noiseSamples[2] = PerlinNoise::Noise(voxPos.x / 32.0f , voxPos.z / 32.0f)	/ 15.0f;
			noiseSamples[3] = PerlinNoise::Noise(voxPos.x / 16.0f , voxPos.z / 16.0f)	/ 20.0f;
			noiseSamples[4] = PerlinNoise::Noise(voxPos.x / 8.0f  , voxPos.z / 8.0f)	/ 25.0f;

			//Get sum of samples
			float noiseSample = 0;
			for (int i = 0; i < sampleCount; i++) {
				noiseSample += noiseSamples[i];
			}
			//Convert from -1 -> 1 range, to 0 -> 1 range
			noiseSample = noiseSample / 2 + 0.5f;

			//Amount of blocks always at bottom of terrain
			const int baseHeight = 4;
			const int remainderHeight = c->GetHeight() - baseHeight;

			//Variables for tapering terrain height for island effect
			Vector2Int middleOfMap = Vector2Int((c->GetWidth() * ChunkHandler::GetMapSize()) / 2, 
												(c->GetDepth() * ChunkHandler::GetMapSize()) / 2);
			Vector2Int currentCol = Vector2Int(c->GetXIndex() * c->GetWidth() + x,
											   c->GetZIndex() * c->GetDepth() + z);
			float dist = sqrt(pow(middleOfMap.x - currentCol.x, 2.0f) + pow(middleOfMap.y - currentCol.y, 2.0f));
			float normalisedDist = 1.0f - sqrt(std::min(dist / middleOfMap.x, 1.0f));

			//Get final terrain height
			int terrainHeight = (int)((baseHeight + (remainderHeight * noiseSample) * std::min(normalisedDist, 0.90f)) + 0.5f);

			//Loop through chunk Y axis until reached the terrain height
			for (int y = 0; y < terrainHeight; y++) {
				Vector3Int pos = Vector3Int(voxPos.x, y, voxPos.z);
				SetVoxel(GetVoxelType(pos, terrainHeight), pos);
			}
		}
	}
}

char WorldManipulation::GetVoxelType(DirectX::SimpleMath::Vector3Int pos, int terrainHeight) {
	if (terrainHeight < 15)
		return VOXEL_TYPE::SAND;

	if (pos.y == terrainHeight - 1)         //Set grass as top layer
		return VOXEL_TYPE::GRASS;
	else if (terrainHeight - pos.y < 5)     //Set dirt layers
		return VOXEL_TYPE::DIRT;
	else                                    //Set remaining as stone
		return VOXEL_TYPE::STONE;
}

DirectX::SimpleMath::Vector3Int WorldManipulation::GetRelativePos(Chunk* c, DirectX::SimpleMath::Vector3Int worldPos) {
	return Vector3Int(worldPos.x - (c->GetXIndex() * c->GetWidth()),
					  worldPos.y,
					  worldPos.z - (c->GetZIndex() * c->GetDepth()));
}
