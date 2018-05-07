#include "Model.h"


//Load a model using assimp
void Model::loadASSIMP(std::string const& path)
{
	//Creates an assimp importer
	Assimp::Importer importer;

	//Creates a assimp scene and imports the model
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs /*| aiProcess_CalcTangentSpace*/);

	//Error checkinig the assimp scene
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		SDL_Log("Error loading model with assimp %s", importer.GetErrorString());;
		return;
	}

	//Creates a assimp mech from the scene rootnode
	aiMesh* mesh = scene->mMeshes[0];

	//Loop through all the vertices to get the vertex data
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		//Vertex container 
		Vertex vertex;

		//Vertex data
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 textureCoordinate;

		//Temp container for holding the vertex data
		glm::vec3 vec3Container;

		//Vertices of the model
		vec3Container.x = mesh->mVertices[i].x;
		vec3Container.y = mesh->mVertices[i].y;
		vec3Container.z = mesh->mVertices[i].z;
		position = vec3Container;

		//Normals of the model
		vec3Container.x = mesh->mNormals[i].x;
		vec3Container.y = mesh->mNormals[i].y;
		vec3Container.z = mesh->mNormals[i].z;
		normal = vec3Container;

		//Texture coordinates of the model if present
		if (mesh->mTextureCoords[0])
		{
			//Temp container for holding the vertex data
			glm::vec2 vec2Container;

			//Texture coordinates of the model
			vec2Container.x = mesh->mTextureCoords[0][i].x;
			vec2Container.y = mesh->mTextureCoords[0][i].y;
			textureCoordinate = vec2Container;
		}

		//Set texture coordinates to 0 if not present
		else textureCoordinate = glm::vec2(0.0f, 0.0f);

		//Assign the vertex data
		vertex.position = position;
		vertex.normal = normal;
		vertex.textureCoordinate = textureCoordinate;

		//Push back the complete vertex to the vertices array
		vertices.push_back(vertex);
	}

	//Loop through all the faces to get the face data
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		//Creat a temp face container
		aiFace face = mesh->mFaces[i];

		//Loop through all the face indices
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
}


//Set the buffers
void Model::setBuffers()
{
	//Generate VAO
	glGenVertexArrays(1, &VAO);

	//Generate VBO and EBO buffers
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//Bind the VAO
	glBindVertexArray(VAO);

	//Bind the VBO buffer data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	//Binf the EBO buffer data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	//Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	//Texture coord attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoordinate));

	//Normals attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	//Unbind the VAO
	glBindVertexArray(0);
}

//Render the model
void Model::render()
{
	//Bind the VAO and draw elements - unbind when complete
	glBindVertexArray(VAO);
	//glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
