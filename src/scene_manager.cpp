#include "scene_manager.hpp"

#include "point_light.hpp"
#include "spot_light.hpp"
#include "dir_light.hpp"
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

int light_id = 0;

void scene_manager::prepare_scene() {
    // OBJECT CREATION
	loadIdentity(MODEL);

    create_frog();
    for(int i = 0; i < 3; i++) {
        create_floor(22.5f - 22.5f * i);
    }
    create_road();
    create_water();

    create_car(-18.5f, -24.0f, 1.0f);
    create_car(-18.5f, -4.0f, 1.0f);
    create_car(-12.5f, 20.0f, -1.0f);
    create_car(-12.5f, 0.0f, -1.0f);
    create_car(-6.5f, -14.0f, 1.0f);
    create_car(-6.5f, 4.0f, 1.0f);

    create_log(2.5f, -25.0f, 0.5f);
    create_log(2.5f, -9.5f, 0.5f);
    create_log(2.5f, 6.0f, 0.5f);
    create_log(6.0f, 21.5f, -0.5f);
    create_log(6.0f, 6.0f, -0.5f);
    create_log(6.0f, -9.5f, -0.5f);
    create_log(9.5f, -25.0f, 0.5f);
    create_log(9.5f, -9.5f, 0.5f);
    create_log(9.5f, 6.0f, 0.5f);
    create_log(13.0f, 21.5f, -0.5f);
    create_log(13.0f, 6.0f, -0.5f);
    create_log(13.0f, -9.5f, -0.5f);
    create_log(16.5f, -25.0f, 0.5f);
    create_log(16.5f, -9.5f, 0.5f);
    create_log(16.5f, 6.0f, 0.5f);

    // LIGHT CREATION
    create_spot();
    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 3; j++) {
            create_point(20.0f - i * 40.0f, 20.0f - j * 20.0f);
        }
    }
    create_dir();
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

void scene_manager::toggle_spot() {
    lights[0]->is_enabled = !lights[0]->is_enabled;
}

void scene_manager::toggle_points() {
    for(int i = 1; i < 7; i++) {
        lights[i]->is_enabled = !lights[i]->is_enabled;
    }
}

void scene_manager::create_frog() {

    float amb[] = { 0.0f, 0.1f, 0.0f, 1.0f };
    float diff[] = { 0.0f, 0.8f, 0.1f, 1.0f };
    float spec[] = { 0.4f, 0.8f, 0.4f, 1.0f };
    float emissive[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float shininess = 10.0f;
    int texcount = 0;

    material mat;
    memcpy(mat.ambient, amb, 4 * sizeof(float));
    memcpy(mat.diffuse, diff, 4 * sizeof(float));
    memcpy(mat.specular, amb, 4 * sizeof(float));
    memcpy(mat.emissive, amb, 4 * sizeof(float));
    mat.shininess = shininess;
    mat.texCount = texcount;
    mesh m = createSphere(2.0f, 5);
    m.mat = mat;

    pushMatrix(MODEL);
    translate(MODEL, -22.0f, 0.0f, 0.0f);
    memcpy(m.transform, mMatrix[MODEL], 16 * sizeof(float));
    popMatrix(MODEL);

    auto frog = std::make_shared<object>(m);
    frog->posx = -22.0f;

    amb[1] = 0.0f;
    diff[0] = 0.1f;
    diff[1] = 0.1f;
    spec[0] = 1.0f;
    spec[1] = 1.0f;
    spec[2] = 1.0f;
    shininess = 100.0f;

    memcpy(mat.ambient, amb, 4 * sizeof(float));
    memcpy(mat.diffuse, diff, 4 * sizeof(float));
    memcpy(mat.specular, spec, 4 * sizeof(float));
    mat.shininess = shininess;
    m = createSphere(0.5f, 3);
    m.mat = mat;

    pushMatrix(MODEL);
    translate(MODEL, 1.2f, 1.2f, 1.2f);
    memcpy(m.transform, mMatrix[MODEL], 16 * sizeof(float));
    popMatrix(MODEL);

    auto r_eye = std::make_shared<object>(m);

    pushMatrix(MODEL);
    translate(MODEL, 1.2f, 1.2f, -1.2f);
    memcpy(m.transform, mMatrix[MODEL], 16 * sizeof(float));
    popMatrix(MODEL);

    auto l_eye = std::make_shared<object>(m);

    frog->add_child(std::move(r_eye));
    frog->add_child(std::move(l_eye));
    frog->set_limits(24.0f, -24.0f, 24.0f, -24.0f);
    objs.push_back(std::move(frog));
}

void scene_manager::create_floor(float offset) {
    float amb[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    float diff[] = { 0.4f, 0.4f, 0.4f, 1.0f };
    float spec[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    float emissive[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float shininess = 10.0f;
    int texcount = 0;

    material mat;
    memcpy(mat.ambient, amb, 4 * sizeof(float));
    memcpy(mat.diffuse, diff, 4 * sizeof(float));
    memcpy(mat.specular, amb, 4 * sizeof(float));
    memcpy(mat.emissive, emissive, 4 * sizeof(float));
    mat.shininess = shininess;
    mat.texCount = texcount;
    mesh m = createQuad(5.0f, 50.0f);
    m.mat = mat;

    pushMatrix(MODEL);
    translate(MODEL, offset, -1.0f, 0.0f);
    rotate(MODEL, -90.0f, 1.0f, 0.0f, 0.0f);
    memcpy(m.transform, mMatrix[MODEL], 16 * sizeof(float));
    popMatrix(MODEL);

    auto floor = std::make_shared<object>(m);
    objs.push_back(std::move(floor));
}

void scene_manager::create_road() {
    float amb[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float diff[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    float spec[] = { 0.15f, 0.15f, 0.15f, 1.0f };
    float emissive[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float shininess = 10.0f;
    int texcount = 0;

    material mat;
    memcpy(mat.ambient, amb, 4 * sizeof(float));
    memcpy(mat.diffuse, diff, 4 * sizeof(float));
    memcpy(mat.specular, amb, 4 * sizeof(float));
    memcpy(mat.emissive, emissive, 4 * sizeof(float));
    mat.shininess = shininess;
    mat.texCount = texcount;
    mesh m = createQuad(17.5f, 50.0f);
    m.mat = mat;

    pushMatrix(MODEL);
    translate(MODEL, -11.25, -1.0f, 0.0f);
    rotate(MODEL, -90.0f, 1.0f, 0.0f, 0.0f);
    memcpy(m.transform, mMatrix[MODEL], 16 * sizeof(float));
    popMatrix(MODEL);

    auto road = std::make_shared<object>(m);
    objs.push_back(std::move(road));
}

void scene_manager::create_water() {
    float amb[] = { 0.0f, 0.0f, 0.2f, 1.0f };
    float diff[] = { 0.0f, 0.0f, 1.0f, 1.0f };
    float spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float emissive[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float shininess = 100.0f;
    int texcount = 0;

    material mat;
    memcpy(mat.ambient, amb, 4 * sizeof(float));
    memcpy(mat.diffuse, diff, 4 * sizeof(float));
    memcpy(mat.specular, amb, 4 * sizeof(float));
    memcpy(mat.emissive, emissive, 4 * sizeof(float));
    mat.shininess = shininess;
    mat.texCount = texcount;
    mesh m = createQuad(17.5f, 50.0f);
    m.mat = mat;

    pushMatrix(MODEL);
    translate(MODEL, 11.25, -1.0f, 0.0f);
    rotate(MODEL, -90.0f, 1.0f, 0.0f, 0.0f);
    memcpy(m.transform, mMatrix[MODEL], 16 * sizeof(float));
    popMatrix(MODEL);

    auto water = std::make_shared<object>(m);
    objs.push_back(std::move(water));
}

void scene_manager::create_car(float x, float y, float dirz) {
    float amb[] = { 0.2f, 0.0f, 0.0f, 1.0f };
    float diff[] = { 0.8f, 0.0f, 0.0f, 1.0f };
    float spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float emissive[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float shininess = 50.0f;
    int texcount = 0;

    material mat;
    memcpy(mat.ambient, amb, 4 * sizeof(float));
    memcpy(mat.diffuse, diff, 4 * sizeof(float));
    memcpy(mat.specular, amb, 4 * sizeof(float));
    memcpy(mat.emissive, emissive, 4 * sizeof(float));
    mat.shininess = shininess;
    mat.texCount = texcount;
    mesh m = createCube();
    m.mat = mat;

    pushMatrix(MODEL);
    translate(MODEL, x, -1.0f, y);
    scale(MODEL, 3.0f, 2.5f, 5.0f);
    memcpy(m.transform, mMatrix[MODEL], 16 * sizeof(float));
    popMatrix(MODEL);

    auto car = std::make_shared<object>(m);
    car->posx = x;
    car->posy = -1.0f;
    car->posz = y;
    car->move(0.0f, 0.0f, dirz);
    car->loop(true);
    car->set_limits(100.0f, -100.0f, 20.0f, -25.0f);
    car->stencil_func = GL_NOTEQUAL;
    objs.push_back(std::move(car));
}

void scene_manager::create_log(float x, float y, float dirz) {
    float amb[] = { 0.2f, 0.1f, 0.0f, 1.0f };
    float diff[] = { 0.5f, 0.25f, 0.1f, 1.0f };
    float spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float emissive[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float shininess = 50.0f;
    int texcount = 0;

    material mat;
    memcpy(mat.ambient, amb, 4 * sizeof(float));
    memcpy(mat.diffuse, diff, 4 * sizeof(float));
    memcpy(mat.specular, amb, 4 * sizeof(float));
    memcpy(mat.emissive, emissive, 4 * sizeof(float));
    mat.shininess = shininess;
    mat.texCount = texcount;
    mesh m = createCube();
    m.mat = mat;

    pushMatrix(MODEL);
    translate(MODEL, x, -3.0f, y);
    scale(MODEL, 3.5f, 2.5f, 5.0f);
    memcpy(m.transform, mMatrix[MODEL], 16 * sizeof(float));
    popMatrix(MODEL);

    auto log = std::make_shared<object>(m);
    log->posx = x;
    log->posy = -1.0f;
    log->posz = y;
    log->move(0.0f, 0.0f, dirz);
    log->loop(true);
    log->set_limits(100.0f, -100.0f, 20.0f, -25.0f);
    log->stencil_func = GL_NOTEQUAL;
    objs.push_back(std::move(log));
}

void scene_manager::create_frog_ai() {

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
        if(texCoords) {
            for(unsigned int k = 0; k < m->mNumVertices; ++k) {

                texCoords[k * 2] = m->mTextureCoords[0][k].x;
                texCoords[k * 2 + 1] = m->mTextureCoords[0][k].y;
            }
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
	auto o1 = std::make_shared<object>(frog);
	objs.push_back(std::move(o1));
}

void scene_manager::create_point(float off_x, float off_z) {
    float pos[3] = { off_x, 6.0f, off_z };
    auto light = std::make_shared<point_light>(light_id, true, pos);

    lights.push_back(std::move(light));
    light_id++;
}

void scene_manager::create_spot() {
    float pos[3] = { 1.0f, 1.0f, 0.0f };
    float dir[3] = { 0.0f, 1.0f, 0.0f };  // something is wrong here
    auto light = std::make_shared<spot_light>(light_id, true, pos, dir, 30.0f);

    lights.push_back(std::move(light));
    light_id++;
}

void scene_manager::create_dir() {
    float dir[3] = { -1.0f, -1.0f, -1.0f };
    auto light = std::make_shared<dir_light>(light_id, true, dir);

    lights.push_back(std::move(light));
    light_id++;
}
