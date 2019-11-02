#version 330

out vec4 colorOut;

struct light_p {
    bool is_enabled;
    bool is_local;
    bool is_spot;
    vec3 position;
    vec3 direction;
    float spot_cutoff;
};

struct Materials {
	vec4 diffuse;
	vec4 ambient;
	vec4 specular;
	vec4 emissive;
	float shininess;
	int texCount;
};

const int MAXLIGHTS = 8;
uniform light_p lights[MAXLIGHTS];

uniform Materials mat;

in Data {
	vec3 normal;
	vec3 eye;
	vec3 o_pos;
} DataIn;

void main() {

    vec4 res = vec4(0.0);

	for (int i = 0; i < MAXLIGHTS; ++i) {
        if (!lights[i].is_enabled || !lights[i].is_local) continue;

        vec4 spec = vec4(0.0);
	    float intensity = 0.0f;
	    float intSpec = 0.0f;

        vec3 n = normalize(DataIn.normal);
	    vec3 l = normalize(lights[i].position - DataIn.o_pos);
	    vec3 e = normalize(DataIn.eye);
	    vec3 sd = normalize(vec3(-lights[i].direction));

        if(lights[i].is_spot)  {                 //Scene iluminated by a spotlight
		    if(dot(l, sd) > lights[i].spot_cutoff)  {
			    intensity = max(dot(n,l), 0.0);
			    if (intensity > 0.0) {
				    vec3 h = normalize(l + e);
				    intSpec = max(dot(h,n), 0.0);
				    spec = mat.specular * pow(intSpec, mat.shininess);
			    }
		    }
	    } else {                                 //Scene iluminated by a pointlight
		    intensity = max(dot(n,l), 0.0);
		    if (intensity > 0.0) {
			    vec3 h = normalize(l + e);
			    intSpec = max(dot(h,n), 0.0);
			    spec = mat.specular * pow(intSpec, mat.shininess);
		    }
	    }

        res += max(intensity * mat.diffuse + spec, mat.ambient);
    }

    colorOut = vec4(res.xyz, 1.0);
}