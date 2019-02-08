#include <engine/voxels/ChunkManager.hpp>
//#include <engine/core/utils/Task.hpp>

#include <algorithm>
#include <utility>

using namespace phx::voxels;

const int VIEW_DISTANCE = 16; // 96 blocks, 6 chunks.

ChunkManager::ChunkManager(const std::string& blockID, int chunkSize, unsigned int seed) :
	m_seed(seed), m_chunkSize(chunkSize),
	m_defaultBlockID(blockID)
{}

void ChunkManager::toggleWireframe()
{
	m_wireframe = !m_wireframe;
	glPolygonMode(GL_FRONT_AND_BACK, m_wireframe ? GL_LINE : GL_FILL);
}

bool ChunkManager::isWireframe() const
{
	return m_wireframe;
}

void ChunkManager::determineGeneration(phx::Vector3 cameraPosition)
{
	cameraPosition = cameraPosition / 2.f;
	cameraPosition += 0.5f;

	const int posX = static_cast<int>(cameraPosition.x) / m_chunkSize;
	const int posY = static_cast<int>(cameraPosition.y) / m_chunkSize;
	const int posZ = static_cast<int>(cameraPosition.z) / m_chunkSize;

	// Get diameter to generate for.
	const int chunkViewDistance = VIEW_DISTANCE / m_chunkSize;

	for (int x = -chunkViewDistance; x <= chunkViewDistance; x++)
	{
		for (int y = -chunkViewDistance; y <= chunkViewDistance; y++)
		{
			for (int z = -chunkViewDistance; z <= chunkViewDistance; z++)
			{
				phx::Vector3 chunkToCheck = {
					static_cast<float>(x) * chunkViewDistance + posX,
					static_cast<float>(y) * chunkViewDistance + posY,
					static_cast<float>(z) * chunkViewDistance + posZ
				};

				chunkToCheck = chunkToCheck * static_cast<float>(m_chunkSize);

				auto result = std::find_if(m_chunks.begin(), m_chunks.end(),
					[chunkToCheck](const Chunk& o) -> bool
					{
						return o.getChunkPos() == chunkToCheck;
					});

				if (result == m_chunks.end())
				{
					m_chunks.emplace_back(chunkToCheck, m_chunkSize, m_defaultBlockID);
					m_chunks.back().populateData(m_seed);
				}
			}
		}
	}
}

void ChunkManager::testGeneration()
{
	for (int i = 0; i < 5; ++i)
	{
		phx::Vector3 pain = { i * 16.f, 0, 0 };

		m_chunks.emplace_back(pain, m_chunkSize, m_defaultBlockID);
		m_chunks.back().populateData(m_seed);
	}
}

void ChunkManager::unloadRedundant()
{
	// TODO this.
}

void ChunkManager::setBlockAt(phx::Vector3 position, const BlockInstance& block)
{
	int posX = static_cast<int>(position.x / m_chunkSize);
	int posY = static_cast<int>(position.y / m_chunkSize);
	int posZ = static_cast<int>(position.z / m_chunkSize);

	position.x = static_cast<float>(static_cast<int>(position.x) % m_chunkSize);
	if (position.x < 0)
	{
		posX -= 1;
		position.x += m_chunkSize;
	}

	position.y = static_cast<float>(static_cast<int>(position.y) % m_chunkSize);
	if (position.y < 0)
	{
		posY -= 1;
		position.y += m_chunkSize;
	}

	position.z = static_cast<float>(static_cast<int>(position.z) % m_chunkSize);
	if (position.z < 0)
	{
		posZ -= 1;
		position.z += m_chunkSize;
	}

	const phx::Vector3 chunkPosition = phx::Vector3(static_cast<float>(posX * m_chunkSize),
		static_cast<float>(posY * m_chunkSize),
		static_cast<float>(posZ * m_chunkSize));

	for (auto& chunk : m_chunks)
	{
		if (chunk.getChunkPos() == chunkPosition)
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

BlockInstance ChunkManager::getBlockAt(phx::Vector3 position) const
{
	int posX = static_cast<int>(position.x / m_chunkSize);
	int posY = static_cast<int>(position.y / m_chunkSize);
	int posZ = static_cast<int>(position.z / m_chunkSize);

	position.x = static_cast<float>(static_cast<int>(position.x) % m_chunkSize);
	if (position.x < 0)
	{
		posX -= 1;
		position.x += m_chunkSize;
	}

	position.y = static_cast<float>(static_cast<int>(position.y) % m_chunkSize);
	if (position.y < 0)
	{
		posY -= 1;
		position.y += m_chunkSize;
	}

	position.z = static_cast<float>(static_cast<int>(position.z) % m_chunkSize);
	if (position.z < 0)
	{
		posZ -= 1;
		position.z += m_chunkSize;
	}

	const phx::Vector3 chunkPosition = phx::Vector3(static_cast<float>(posX * m_chunkSize),
		static_cast<float>(posY * m_chunkSize),
		static_cast<float>(posZ * m_chunkSize));

	for (auto& chunk : m_chunks)
	{
		if (chunk.getChunkPos() == chunkPosition)
		{
			return chunk.getBlockAt(
				{ // "INLINE" VECTOR 3 DECLARATION
				position.x, // x position IN the chunk, not overall 
				position.y, // y position IN the chunk, not overall 
				position.z  // z position IN the chunk, not overall 
				}
			);
		}
	}

	return BlockInstance("core:out_of_bounds");
}

void ChunkManager::breakBlockAt(phx::Vector3 position, const BlockInstance& block)
{
	int posX = static_cast<int>(position.x / m_chunkSize);
	int posY = static_cast<int>(position.y / m_chunkSize);
	int posZ = static_cast<int>(position.z / m_chunkSize);

	position.x = static_cast<float>(static_cast<int>(position.x) % m_chunkSize);
	if (position.x < 0)
	{
		posX -= 1;
		position.x += m_chunkSize;
	}

	position.y = static_cast<float>(static_cast<int>(position.y) % m_chunkSize);
	if (position.y < 0)
	{
		posY -= 1;
		position.y += m_chunkSize;
	}

	position.z = static_cast<float>(static_cast<int>(position.z) % m_chunkSize);
	if (position.z < 0)
	{
		posZ -= 1;
		position.z += m_chunkSize;
	}

	const phx::Vector3 chunkPosition = phx::Vector3(static_cast<float>(posX * m_chunkSize),
		static_cast<float>(posY * m_chunkSize),
		static_cast<float>(posZ * m_chunkSize));

	for (auto& chunk : m_chunks)
	{
		if (chunk.getChunkPos() == chunkPosition)
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
	int posX = static_cast<int>(position.x / m_chunkSize);
	int posY = static_cast<int>(position.y / m_chunkSize);
	int posZ = static_cast<int>(position.z / m_chunkSize);

	position.x = static_cast<float>(static_cast<int>(position.x) % m_chunkSize);
	if (position.x < 0)
	{
		posX -= 1;
		position.x += m_chunkSize;
	}

	position.y = static_cast<float>(static_cast<int>(position.y) % m_chunkSize);
	if (position.y < 0)
	{
		posY -= 1;
		position.y += m_chunkSize;
	}

	position.z = static_cast<float>(static_cast<int>(position.z) % m_chunkSize);
	if (position.z < 0)
	{
		posZ -= 1;
		position.z += m_chunkSize;
	}

	const phx::Vector3 chunkPosition = phx::Vector3(static_cast<float>(posX * m_chunkSize),
		static_cast<float>(posY * m_chunkSize),
		static_cast<float>(posZ * m_chunkSize));

	for (auto& chunk : m_chunks)
	{
		if (chunk.getChunkPos() == chunkPosition)
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

	for (Chunk& chunk : m_chunks)
	{
		chunk.renderBlocks(&count1);
	}
}
