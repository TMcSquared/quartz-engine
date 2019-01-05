#include <engine/voxels/ChunkManager.hpp>

#include <algorithm>

using namespace phx::voxels;

ChunkManager::ChunkManager(const std::string& blockID, unsigned int seed) :
	m_seed(seed),
	m_defaultBlockID(blockID)
{
	m_managerData = new ChunkContainer();
}

void ChunkManager::toggleWireframe()
{
	m_wireframe = !m_wireframe;
	glPolygonMode(GL_FRONT_AND_BACK, m_wireframe ? GL_LINE : GL_FILL);
}

bool ChunkManager::isWireframe()
{
	return m_wireframe;
}

void ChunkManager::determineGeneration(phx::Vector3 cameraPosition)
{
	cameraPosition = cameraPosition / 2.f;
	cameraPosition += 0.5f;

	int posX = static_cast<int>(cameraPosition.x) / 16;
	int posY = static_cast<int>(cameraPosition.y) / 16;
	int posZ = static_cast<int>(cameraPosition.z) / 16;

	const int VIEW_DISTANCE = 16; // 96 blocks, 6 chunks.

	int chunkViewDistance = (VIEW_DISTANCE / 16) * 2;

	for (int x = -chunkViewDistance; x <= chunkViewDistance; x++)
	{
		for (int y = -chunkViewDistance; y <= chunkViewDistance; y++)
		{
			for (int z = -chunkViewDistance; z <= chunkViewDistance; z++)
			{
				phx::Vector3 chunkToCheck = {
					static_cast<float>(x * chunkViewDistance / 2 + posX),
					static_cast<float>(y * chunkViewDistance / 2 + posY),
					static_cast<float>(z * chunkViewDistance / 2 + posZ)
				};

				chunkToCheck = chunkToCheck * 16;

				auto result = std::find(m_managerData->positions.begin(), m_managerData->positions.end(), chunkToCheck);
				
				if (result == m_managerData->positions.end())
				{
					LDEBUG("Chunk Position at:", chunkToCheck.x, " ", chunkToCheck.y, " ", chunkToCheck.z);
					LDEBUG("CHUNK DOES NOT EXIST. GENERATING NEW CHUNK AT POSITION");
					m_managerData->chunks.emplace_back(chunkToCheck, 16, m_defaultBlockID);
					m_managerData->chunks.back().populateData(m_seed);
					m_managerData->positions.push_back(chunkToCheck);
				}
			}
		}
	}
}

void ChunkManager::testGeneration(int test)
{
	for (int x = 0; x < test; x++)
	{
		for (int z = 0; z < test; z++)
		{
			m_managerData->chunks.emplace_back(Chunk({ x * 16.f, 0.f, z * 16.f }, 16, "core:air"));
			m_managerData->chunks.back().populateData(m_seed);
			LDEBUG("Generating chunk.");
		}
	}
}

void ChunkManager::unloadRedundant()
{
	/* TODO this. */
}

void ChunkManager::setBlockAt(phx::Vector3 position, const BlockInstance& block)
{
	int posX = static_cast<int>(position.x / 16);
	int posY = static_cast<int>(position.y / 16);
	int posZ = static_cast<int>(position.z / 16);

	position.x = static_cast<float>(static_cast<int>(position.x) % 16);
	if (position.x < 0)
	{
		posX -= 1;
		position.x += 16;
	}

	position.y = static_cast<float>(static_cast<int>(position.y) % 16);
	if (position.y < 0)
	{
		posY -= 1;
		position.y += 16;
	}

	position.z = static_cast<float>(static_cast<int>(position.z) % 16);
	if (position.z < 0)
	{
		posZ -= 1;
		position.z += 16;
	}

	phx::Vector3 temp = phx::Vector3(posX * 16.f, posY * 16.f, posZ * 16.f);

	for (auto& chunk : m_managerData->chunks)
	{
		if (chunk.getChunkPos() == temp)
		{
			chunk.setBlockAt(
				{ // "INLINE" VECTOR 3 DECLARATION
				position.x, // x position IN the chunk, not overall 
				position.y, // y position IN the chunk, not overall 
				position.z  // z position IN the chunk, not overall 
				}, 
				block
			);

			break;
		}
	}
}

BlockInstance ChunkManager::getBlockAt(phx::Vector3 position)
{
	int pos_x = static_cast<int>(position.x / 16);
	int pos_y = static_cast<int>(position.y / 16);
	int pos_z = static_cast<int>(position.z / 16);

	position.x = static_cast<float>(static_cast<int>(position.x) % 16);
	if (position.x < 0)
	{
		pos_x -= 1;
		position.x += 16;
	}

	position.y = static_cast<float>(static_cast<int>(position.y) % 16);
	if (position.y < 0)
	{
		pos_y -= 1;
		position.y += 16;
	}

	position.z = static_cast<float>(static_cast<int>(position.z) % 16);
	if (position.z < 0)
	{
		pos_z -= 1;
		position.z += 16;
	}

	phx::Vector3 temp = phx::Vector3(pos_x * 16.f, pos_y * 16.f, pos_z * 16.f);

	for (auto& chunk : m_managerData->chunks)
	{
		if (chunk.getChunkPos() == temp)
		{
			return chunk.getBlockAt(
				{ // "INLINE" VECTOR 3 DECLARATION
				position.x, // x position IN the chunk, not overall 
				position.y, // y position IN the chunk, not overall 
				position.z  // z position IN the chunk, not overall 
				}
			);

			break;
		}
	}

	return BlockInstance("core:out_of_bounds");
}

void ChunkManager::breakBlockAt(phx::Vector3 position, const BlockInstance& block)
{
	int posX = static_cast<int>(position.x / 16);
	int posY = static_cast<int>(position.y / 16);
	int posZ = static_cast<int>(position.z / 16);

	position.x = static_cast<float>(static_cast<int>(position.x) % 16);
	if (position.x < 0)
	{
		posX -= 1;
		position.x += 16;
	}

	position.y = static_cast<float>(static_cast<int>(position.y) % 16);
	if (position.y < 0)
	{
		posY -= 1;
		position.y += 16;
	}

	position.z = static_cast<float>(static_cast<int>(position.z) % 16);
	if (position.z < 0)
	{
		posZ -= 1;
		position.z += 16;
	}

	phx::Vector3 temp = phx::Vector3(posX * 16.f, posY * 16.f, posZ * 16.f);

	for (auto& chunk : m_managerData->chunks)
	{
		if (chunk.getChunkPos() == temp)
		{
			chunk.breakBlockAt(
				{ // "INLINE" VECTOR 3 DECLARATION
				position.x, // x position IN the chunk, not overall 
				position.y, // y position IN the chunk, not overall 
				position.z  // z position IN the chunk, not overall 
				},
				block
			);

			break;
		}
	}
}

void ChunkManager::placeBlockAt(phx::Vector3 position, const BlockInstance& block)
{
	int posX = static_cast<int>(position.x / 16);
	int posY = static_cast<int>(position.y / 16);
	int posZ = static_cast<int>(position.z / 16);

	position.x = static_cast<float>(static_cast<int>(position.x) % 16);
	if (position.x < 0)
	{
		posX -= 1;
		position.x += 16;
	}

	position.y = static_cast<float>(static_cast<int>(position.y) % 16);
	if (position.y < 0)
	{
		posY -= 1;
		position.y += 16;
	}

	position.z = static_cast<float>(static_cast<int>(position.z) % 16);
	if (position.z < 0)
	{
		posZ -= 1;
		position.z += 16;
	}

	phx::Vector3 temp = phx::Vector3(posX * 16.f, posY * 16.f, posZ * 16.f);
	LDEBUG("Attempting to place block at: ", position.x, " ", position.y, " ", position.z, " in Chunk: ", temp.x, " ", temp.y, " ", temp.z);

	for (auto& chunk : m_managerData->chunks)
	{
		if (chunk.getChunkPos() == temp)
		{
			chunk.placeBlockAt(
				{ // "INLINE" VECTOR 3 DECLARATION
				position.x, // x position IN the chunk, not overall 
				position.y, // y position IN the chunk, not overall 
				position.z  // z position IN the chunk, not overall 
				},
				block
			);

			break;
		}
	}
}

void ChunkManager::render(int bufferCounter)
{
	int count1 = bufferCounter;

	for (auto& chunk : m_managerData->chunks)
	{
		chunk.renderBlocks(&count1);
	}
}