#include <engine/common.hpp>

#include "client/chunk.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

using namespace pheonix::graphics;

extern const char* vertexShaderSource;
const char* vertexShaderSource = "#version 330 core \n"
								 "layout (location = 0) in vec3 aPos; \n"
								 "layout (location = 1) in vec2 aUV; \n"
								 "uniform mat4 model; \n"
								 "uniform mat4 view; \n"
								 "uniform mat4 projection; \n"
								 "out vec2 UV; \n"
								 "void main() \n"
								 "{ \n"
								 "gl_Position = projection * view * model * vec4(aPos, 1.0); \n"
								 "UV = aUV; \n"
								 "}";

extern const char* fragmentShaderSource;
const char* fragmentShaderSource = "#version 330 core \n"
								   "out vec4 FragColor; \n"
								   "in vec2 UV; \n"
								   "uniform sampler2D theTexture; \n"
								   "void main() \n"
								   "{ \n"
								   "vec4 tex = texture(theTexture, UV);"
								   "FragColor = tex;"
								   "}";

static const glm::vec3 CubeVerts[] = {
	// front
	glm::vec3(-1.f,-1.f,-1.f),
	glm::vec3(1.f,-1.f,-1.f),
	glm::vec3(1.f,1.f,-1.f),
	glm::vec3(1.f,1.f,-1.f),
	glm::vec3(-1.f,1.f,-1.f),
	glm::vec3(-1.f,-1.f,-1.f),

	// back
	glm::vec3(-1.f,-1.f,1.f),
	glm::vec3(1.f,-1.f,1.f),
	glm::vec3(1.f, 1.f,1.f),
	glm::vec3(1.f, 1.f,1.f),
	glm::vec3(-1.f, 1.f,1.f),
	glm::vec3(-1.f, -1.f,1.f),

	// left
	glm::vec3(-1.f, 1.f,1.f),
	glm::vec3(-1.f, 1.f,-1.f),
	glm::vec3(-1.f, -1.f,-1.f),
	glm::vec3(-1.f, -1.f,-1.f),
	glm::vec3(-1.f, -1.f,1.f),
	glm::vec3(-1.f, 1.f,1.f),

	// right
	glm::vec3(1.f, 1.f,1.f),
	glm::vec3(1.f, 1.f,-1.f),
	glm::vec3(1.f, -1.f,-1.f),
	glm::vec3(1.f, -1.f,-1.f),
	glm::vec3(1.f, -1.f,1.f),
	glm::vec3(1.f, 1.f,1.f),

	// bottom
	glm::vec3(-1.f, -1.f, -1.f),
	glm::vec3(1.f, -1.f, -1.f),
	glm::vec3(1.f, -1.f,  1.f),
	glm::vec3(1.f, -1.f,  1.f),
	glm::vec3(-1.f, -1.f,  1.f),
	glm::vec3(-1.f, -1.f, -1.f),

	// top
	glm::vec3(-1.f,  1.f, -1.f),
	glm::vec3(1.f,  1.f, -1.f),
	glm::vec3(1.f,  1.f,  1.f),
	glm::vec3(1.f,  1.f,  1.f),
	glm::vec3(-1.f,  1.f,  1.f),
	glm::vec3(-1.f,  1.f, -1.f)
};

static const glm::vec3 CubeVertEmpty[] = {
	glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f),
	glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f),
	glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f),
	glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f),
	glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f),
	glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f),
	glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f),
	glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f),
	glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f),
	glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f),
	glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f),
	glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f)
};

static const glm::vec2 CubeUV[] = {
	// front
	glm::vec2(0.f, 0.f),
	glm::vec2(1.f, 0.f),
	glm::vec2(1.f, 1.f),
	glm::vec2(1.f, 1.f),
	glm::vec2(0.f, 1.f),
	glm::vec2(0.f, 0.f),

	// back
	glm::vec2(0.f, 0.f),
	glm::vec2(1.f, 0.f),
	glm::vec2(1.f, 1.f),
	glm::vec2(1.f, 1.f),
	glm::vec2(0.f, 1.f),
	glm::vec2(0.f, 0.f),

	// left
	glm::vec2(1.f, 0.f),
	glm::vec2(1.f, 1.f),
	glm::vec2(0.f, 1.f),
	glm::vec2(0.f, 1.f),
	glm::vec2(0.f, 0.f),
	glm::vec2(1.f, 0.f),

	// right
	glm::vec2(1.f, 0.f),
	glm::vec2(1.f, 1.f),
	glm::vec2(0.f, 1.f),
	glm::vec2(0.f, 1.f),
	glm::vec2(0.f, 0.f),
	glm::vec2(1.f, 0.f),

	// bottom
	glm::vec2(0.f, 1.f),
	glm::vec2(1.f, 1.f),
	glm::vec2(1.f, 0.f),
	glm::vec2(1.f, 0.f),
	glm::vec2(0.f, 0.f),
	glm::vec2(0.f, 1.f),

	// top
	glm::vec2(0.f, 1.f),
	glm::vec2(1.f, 1.f),
	glm::vec2(1.f, 0.f),
	glm::vec2(1.f, 0.f),
	glm::vec2(0.f, 0.f),
	glm::vec2(0.f, 1.f)
};


Chunk::Chunk() :
	m_vao( new opengl::VertexArray() ),
	m_vbo( new opengl::Buffer( opengl::Buffer::Target::ARRAY, opengl::Buffer::Usage::DYNAMIC_DRAW ) ),
	m_uvbo( new opengl::Buffer( opengl::Buffer::Target::ARRAY, opengl::Buffer::Usage::DYNAMIC_DRAW ) )
{
}

void Chunk::populateChunk( unsigned int chunkSize ) {
	unsigned int sizeCubed = chunkSize * chunkSize * chunkSize;

	m_chunkSize = chunkSize;
	m_vertsInChunk = sizeCubed * vertInCube;
	m_uvsInChunk = uvInCube * sizeCubed;

	// Set whole array to have, for example, 16 parts inside the vector.
	m_chunkBlocks.resize( chunkSize );

	for (unsigned int x = 0; x < m_chunkBlocks.size(); x++)
	{
		// Set each X index (first vector part, of the trio) to have, for example, 16 parts. So you can have m_chunkBlocks[x][0] to m_chunkBlocks[x][15]
		m_chunkBlocks[x].resize( chunkSize );
		for (unsigned int y = 0; y < m_chunkBlocks.size(); y++)
		{
			// Set the Y (first vector part, of the trio) to have, for example, 16 parts. So you can have m_chunkBlocks[x][y][0] to m_chunkBlocks[x][y][15]
			m_chunkBlocks[x][y].resize( chunkSize );
			for (unsigned int z = 0; z < m_chunkBlocks.size(); z++)
			{
				// Set the Z (first vector part, of the trio) to have the actual value of the blocks data. So, you can have m_chunkBlocks[x][y][z] = BlockType::SOLID
				m_chunkBlocks[x][y][z] = BlockType::SOLID;
			}
		}
	}
}

void Chunk::build() {

	//    if ( m_populated )
	//        this->clearOpenGL();

	m_chunkVertices = new glm::vec3[ m_vertsInChunk ];
	glm::vec2*   chunkUVs = new glm::vec2[ m_uvsInChunk ];

	for (int z = 0; z < m_chunkSize; z++)
	{
		for (int y = 0; y < m_chunkSize; y++)
		{
			for (int x = 0; x < m_chunkSize; x++)
			{
				int memOffset = ( x * vertInCube ) + (m_chunkSize * ((y * vertInCube) + m_chunkSize * (z * vertInCube)));

				std::memcpy( m_chunkVertices + memOffset, CubeVerts, sizeof( CubeVerts ) );
				std::memcpy( chunkUVs + memOffset, CubeUV, sizeof( CubeUV ) );

				for (int face = 0; face < 6; face++)
				{
					int memOffsetOffest = static_cast<int>( face ) * 6;

					for (int q = memOffset + memOffsetOffest; q < memOffset + memOffsetOffest + 6; q++)
					{
						m_chunkVertices[q].x += x * 2;
						m_chunkVertices[q].y += y * 2;
						m_chunkVertices[q].z += z * 2;
					}
				}
			}
		}
	}

	m_vao->bind();

	m_vbo->bind();
	m_vbo->setData( static_cast<void*>(m_chunkVertices), m_vertsInChunk * sizeof(glm::vec3) );
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);

	m_uvbo->bind();
	m_uvbo->setData( static_cast<void*>(chunkUVs), m_uvsInChunk * sizeof(glm::vec2) );
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
	glEnableVertexAttribArray(1);

	unsigned int vertexShader;
	vertexShader = glCreateShader( GL_VERTEX_SHADER );
	glShaderSource( vertexShader, 1, &vertexShaderSource, nullptr );
	glCompileShader( vertexShader );

	int success;
	char infoLog[512];
	glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &success );

	if ( !success ) {
		glGetShaderInfoLog( vertexShader, 512, nullptr, infoLog );
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	unsigned int fragmentShader;
	fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( fragmentShader, 1, &fragmentShaderSource, nullptr );
	glCompileShader( fragmentShader );

	int success2;
	char infoLog2[512];
	glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &success2 );

	if ( !success2 ) {
		glGetShaderInfoLog( fragmentShader, 512, nullptr, infoLog2 );
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog2 << std::endl;
	}

	m_shaderProgram = glCreateProgram();

	glAttachShader( m_shaderProgram, vertexShader );
	glAttachShader( m_shaderProgram, fragmentShader );
	glLinkProgram( m_shaderProgram );

	glDeleteShader( vertexShader );
	glDeleteShader( fragmentShader );

}

void Chunk::draw()
{
	m_vao->bind();

	glm::mat4 projection = glm::mat4( 1.0f );
	projection = glm::perspective( glm::radians( 45.0f ), 1280.f / 720.f, 0.1f, 100.0f );

	glm::mat4 view = glm::lookAt(
				glm::vec3(-25,50,3), // Camera is at (4,3,3), in World Space
				glm::vec3(10,13,5), // and looks at the origin
				glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
				);

	glm::mat4 model = glm::mat4( 1.0f );

	int modelLoc = glGetUniformLocation( m_shaderProgram, "model" );
	glUniformMatrix4fv( modelLoc, 1, GL_FALSE, glm::value_ptr( model ) );

	int viewLoc = glGetUniformLocation( m_shaderProgram, "view" );
	glUniformMatrix4fv( viewLoc, 1, GL_FALSE, glm::value_ptr( view ) );

	int projectionLoc = glGetUniformLocation( m_shaderProgram, "projection" );
	glUniformMatrix4fv( projectionLoc, 1, GL_FALSE, glm::value_ptr( projection ) );

	glUseProgram( m_shaderProgram );
	m_vao->bind();

	glDrawArrays(GL_TRIANGLES, 0, m_vertsInChunk);
}
