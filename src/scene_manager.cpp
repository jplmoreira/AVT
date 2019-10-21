#include "scene_manager.hpp"

#include "mesh.hpp"
#include "basic_geometry.h"
#include "AVTmathLib.h"

extern float mMatrix[COUNT_MATRICES][16];

void scene_manager::prepare_scene() {

    loadIdentity(MODEL);

    float amb1[] = { 0.15f, 1.0f, 0.5f, 1.0f };
    float diff1[] = { 0.2f, 1.0f, 0.5f, 1.0f };
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





	float spec5[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	float amb5[] = { 0.3f, 0.8f, 0.3f, 1.0f };
	float diff5[] = { 0.3f, 0.8f, 0.3f, 1.0f };

	material mat5;

	memcpy(mat5.ambient, amb5, 4 * sizeof(float));
	memcpy(mat5.diffuse, diff5, 4 * sizeof(float));
	memcpy(mat5.specular, spec5, 4 * sizeof(float));
	memcpy(mat5.emissive, emissive1, 4 * sizeof(float));
	mat5.shininess = shininess1;
	mat5.texCount = texcount1;
	mesh m5 = createQuad(100.0f, 150.0f);
	m5.mat = mat5;

	pushMatrix(MODEL);
	translate(MODEL, -47.5f, -2.0f, 0.0f);
	rotate(MODEL, -90.0f, 1.0f, 0.0f, 0.0f);
	memcpy(m5.transform, mMatrix[MODEL], 16 * sizeof(float));
	popMatrix(MODEL);


	auto floor = std::make_unique<object>(m5);
	objs.push_back(std::move(floor));





    float spec3[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	float amb3[] = { 0.5f, 1.0f, 1.0f, 1.0f };
	float diff3[] = { 0.5f, 1.0f, 1.0f, 1.0f };

    material mat3;
	
    memcpy(mat3.ambient, amb3, 4 * sizeof(float));
    memcpy(mat3.diffuse, diff3, 4 * sizeof(float));
    memcpy(mat3.specular, spec3, 4 * sizeof(float));
    memcpy(mat3.emissive, emissive1, 4 * sizeof(float));
    mat3.shininess = shininess1;
    mat3.texCount = texcount1;
    mesh m3 = createQuad(5.0f, 150.0f);
    m3.mat = mat3;

    pushMatrix(MODEL);
    translate(MODEL, 5.0f, -2.0f, 0.0f);
    rotate(MODEL, -90.0f, 1.0f, 0.0f, 0.0f);
    memcpy(m3.transform, mMatrix[MODEL], 16 * sizeof(float));
    popMatrix(MODEL);


	auto river = std::make_unique<object>(m3);
	objs.push_back(std::move(river));


	float spec4[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	float amb4[] = { 0.7f, 0.4f, 0.3f, 1.0f };
	float diff4[] = { 0.7f, 0.4f, 0.3f, 1.0f };


	material mat4;

	memcpy(mat4.ambient, amb4, 4 * sizeof(float));
	memcpy(mat4.diffuse, diff4, 4 * sizeof(float));
	memcpy(mat4.specular, spec4, 4 * sizeof(float));
	memcpy(mat4.emissive, emissive1, 4 * sizeof(float));
	mat4.shininess = shininess1;
	mat4.texCount = texcount1;
	mesh m4 = createCylinder(15.0f, 2.0f, 16);
	m4.mat = mat4;



	pushMatrix(MODEL);

	translate(MODEL, 5.0f, -2.0f, 0.0f);
	rotate(MODEL, -90.0f, 1.0f, 0.0f, 0.0f);
	memcpy(m4.transform, mMatrix[MODEL], 16 * sizeof(float));
	popMatrix(MODEL);

    auto wood = std::make_unique<object>(m4);
    objs.push_back(std::move(wood));



	float spec6[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	float amb6[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	float diff6[] = { 0.5f, 0.5f, 0.5f, 1.0f };

	material mat6;

	memcpy(mat6.ambient, amb6, 4 * sizeof(float));
	memcpy(mat6.diffuse, diff6, 4 * sizeof(float));
	memcpy(mat6.specular, spec6, 4 * sizeof(float));
	memcpy(mat6.emissive, emissive1, 4 * sizeof(float));
	mat6.shininess = shininess1;
	mat6.texCount = texcount1;
	mesh m6 = createQuad(5.0f, 150.0f);
	m6.mat = mat6;

	pushMatrix(MODEL);
	translate(MODEL, 15.0f, -2.0f, 0.0f);
	rotate(MODEL, -90.0f, 1.0f, 0.0f, 0.0f);
	memcpy(m6.transform, mMatrix[MODEL], 16 * sizeof(float));
	popMatrix(MODEL);


	auto road = std::make_unique<object>(m6);
	objs.push_back(std::move(road));



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

float* scene_manager::wood_pos() {
	float pos[3] = { objs[3]->posx, objs[3]->posy, objs[3]->posz };
	return pos;
}

void scene_manager::wood_stop() {
	objs[3]->move(0.0f, 0.0f, 0.0f);
}


void scene_manager::wood_move() {
	objs[3]->move(0.0f, 0.0f, 1.0f);
}

void scene_manager::wood_inverse_move() {
	objs[3]->move(0.0f, 0.0f, -1.0f);
}


float* scene_manager::river_pos() {
	float pos[3] = { objs[2]->posx, objs[2]->posy, objs[2]->posz };
	return pos;
}

float* scene_manager::road_pos() {
	float pos[3] = { objs[4]->posx, objs[4]->posy, objs[4]->posz };
	return pos;
}





