#include "scene_manager.hpp"

#include "point_light.hpp"
#include "mesh.hpp"
#include "basic_geometry.h"
#include "AVTmathLib.h"
#include "VertexAttrDef.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <fstream>
#include <iostream>
#include <string>

extern float mMatrix[COUNT_MATRICES][16];

void scene_manager::prepare_scene() {
    // OBJECT CREATION
	loadIdentity(MODEL);

	float amb1[] = { 0.0f, 0.2f, 0.0f, 1.0f };
	float diff1[] = { 0.0f, 0.8f, 0.1f, 1.0f };
	float spec1[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	float emissive1[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess1 = 100.0f;
	int texcount1 = 0;

	material mat1;
	memcpy(mat1.ambient, amb1, 4 * sizeof(float));
	memcpy(mat1.diffuse, diff1, 4 * sizeof(float));
	memcpy(mat1.specular, amb1, 4 * sizeof(float));
	memcpy(mat1.emissive, amb1, 4 * sizeof(float));
	mat1.shininess = shininess1;
	mat1.texCount = texcount1;
	mesh m1 = createSphere(2.0f, 5);
	m1.mat = mat1;

	pushMatrix(MODEL);
	memcpy(m1.transform, mMatrix[MODEL], 16 * sizeof(float));
	popMatrix(MODEL);

	auto frog = std::make_unique<object>(m1);

	float amb2[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float diff2[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float spec2[] = { 0.8f, 0.8f, 0.8f, 1.0f };

	material mat2;
	memcpy(mat2.ambient, amb2, 4 * sizeof(float));
	memcpy(mat2.diffuse, diff2, 4 * sizeof(float));
	memcpy(mat2.specular, spec2, 4 * sizeof(float));
	memcpy(mat2.emissive, emissive1, 4 * sizeof(float));
	mat2.shininess = shininess1;
	mat2.texCount = texcount1;
	mesh m2 = createSphere(0.5f, 3);
	m2.mat = mat2;

	pushMatrix(MODEL);
	translate(MODEL, 1.2f, 1.2f, 1.2f);
	memcpy(m2.transform, mMatrix[MODEL], 16 * sizeof(float));
	popMatrix(MODEL);

	auto r_eye = std::make_unique<object>(m2);

	pushMatrix(MODEL);
	translate(MODEL, 1.2f, 1.2f, -1.2f);
	memcpy(m2.transform, mMatrix[MODEL], 16 * sizeof(float));
	popMatrix(MODEL);

	auto l_eye = std::make_unique<object>(m2);

	frog->add_child(std::move(r_eye));
	frog->add_child(std::move(l_eye));
	objs.push_back(std::move(frog));

	float amb3[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float diff3[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	float spec3[] = { 0.8f, 0.8f, 0.8f, 1.0f };

	material mat3;
	memcpy(mat3.ambient, amb3, 4 * sizeof(float));
	memcpy(mat3.diffuse, diff3, 4 * sizeof(float));
	memcpy(mat3.specular, spec3, 4 * sizeof(float));
	memcpy(mat3.emissive, emissive1, 4 * sizeof(float));
	mat3.shininess = shininess1;
	mat3.texCount = texcount1;
	mesh m3 = createQuad(50.0f, 50.0f);
	m3.mat = mat3;

	pushMatrix(MODEL);
	translate(MODEL, 0.0f, -2.0f, 0.0f);
	rotate(MODEL, -90.0f, 1.0f, 0.0f, 0.0f);
	memcpy(m3.transform, mMatrix[MODEL], 16 * sizeof(float));
	popMatrix(MODEL);

	auto floor = std::make_unique<object>(m3);
	objs.push_back(std::move(floor));

    // LIGHT CREATION
    int lightId = 0;

    float pos[3] = { 4.0f, 6.0f, 2.0f };
    //float dir[3] = { -1.0f, 2.0f, 0.0f };  // something is wrong here
    auto light = std::make_unique<point_light>(lightId, true, pos);
    light->color[0] = 0.8f;
    light->color[1] = 0.8f;
    light->color[2] = 0.8f;

    lights.push_back(std::move(light));

    lightId++;
}

void scene_manager::player_forward() {
    objs[0]->move(1.0f, 0.0f, 0.0f);
}

void scene_manager::player_back() {
    objs[0]->move(-1.0f, 0.0f, 0.0f);
}

void scene_manager::player_right() { 
    objs[0]->move(0.0f, 0.0f, 1.0f); 
}

void scene_manager::player_left() {
    objs[0]->move(0.0f, 0.0f, -1.0f);
}

void scene_manager::player_stop() {
    objs[0]->move(0.0f, 0.0f, 0.0f);
}

float* scene_manager::player_pos() {
    float pos[3] = { objs[0]->posx, objs[0]->posy, objs[0]->posz };
    return pos;
}

void scene_manager::create_frog() {
	loadIdentity(MODEL);

	Assimp::Importer importer;

	std::string file = "models/frog.obj";
	std::ifstream fin(file.c_str());
	if (!fin.fail())
		fin.close();
	else {
		std::cout << "Could not open file " << file << std::endl;
		std::cout << importer.GetErrorString() << std::endl;
		return;
	}

	const aiScene* sc = importer.ReadFile(file, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!sc) {
		std::cout << "Error importing file " << file << std::endl;
		std::cout << importer.GetErrorString() << std::endl;
		return;
	}

	aiMesh* m = sc->mMeshes[0];
	aiMaterial* mtl = sc->mMaterials[m->mMaterialIndex];
	material m1;
	aiColor4D color;
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &color)) {
		float d[4] = { color.r, color.g, color.b, color.a };
		memcpy(m1.diffuse, d, sizeof(d));
	}
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &color)) {
		float a[4] = { color.r, color.g, color.b, color.a };
		memcpy(m1.ambient, a, sizeof(a));
	}
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &color)) {
		float s[4] = { color.r, color.g, color.b, color.a };
		memcpy(m1.specular, s, sizeof(s));
	}
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &color)) {
		float e[4] = { color.r, color.g, color.b, color.a };
		memcpy(m1.emissive, e, sizeof(e));
	}
	float shininess = 0.0;
	unsigned int max;
	aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
	m1.shininess = shininess;

	mesh frog;
	GLuint buffer;
	frog.numIndexes = m->mNumFaces * 3;
	glGenVertexArrays(1, &(frog.vao));
	glBindVertexArray(frog.vao);

	unsigned int* faceArray;
	faceArray = (unsigned int*)malloc(sizeof(unsigned int) * m->mNumFaces * 3);
	unsigned int faceIndex = 0;

	for (unsigned int t = 0; t < m->mNumFaces; ++t) {
		const aiFace* face = &m->mFaces[t];

		memcpy(&faceArray[faceIndex], face->mIndices, 3 * sizeof(unsigned int));
		faceIndex += 3;
	}
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m->mNumFaces * 3, faceArray, GL_STATIC_DRAW);

	// buffer for vertex positions
	if (m->HasPositions()) {
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * m->mNumVertices, m->mVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(VERTEX_COORD_ATTRIB);
		glVertexAttribPointer(VERTEX_COORD_ATTRIB, 3, GL_FLOAT, 0, 0, 0);
	}

	// buffer for vertex normals
	if (m->HasNormals()) {
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * m->mNumVertices, m->mNormals, GL_STATIC_DRAW);
		glEnableVertexAttribArray(NORMAL_ATTRIB);
		glVertexAttribPointer(NORMAL_ATTRIB, 3, GL_FLOAT, 0, 0, 0);
	}

	// buffer for vertex texture coordinates
	if (m->HasTextureCoords(0)) {
		float* texCoords = (float*)malloc(sizeof(float) * 2 * m->mNumVertices);
		for (unsigned int k = 0; k < m->mNumVertices; ++k) {

			texCoords[k * 2] = m->mTextureCoords[0][k].x;
			texCoords[k * 2 + 1] = m->mTextureCoords[0][k].y;

		}
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * m->mNumVertices, texCoords, GL_STATIC_DRAW);
		glEnableVertexAttribArray(TEXTURE_COORD_ATTRIB);
		glVertexAttribPointer(TEXTURE_COORD_ATTRIB, 2, GL_FLOAT, 0, 0, 0);
	}

	// unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	frog.type = GL_TRIANGLES;
	frog.mat = m1;
	pushMatrix(MODEL);
	memcpy(frog.transform, mMatrix[MODEL], 16 * sizeof(float));
	popMatrix(MODEL);
	auto o1 = std::make_unique<object>(frog);
	objs.push_back(std::move(o1));
}
