#version 330

out vec4 colorOut;

struct light_p {
    bool is_enabled;
    bool is_local;
    bool is_spot;
    vec3 color;
    vec3 position;
    vec3 direction;
    float spot_cutoff;
    float spot_exponent;
    float constant_attenuation;
    float linear_attenuation;
    float quadratic_attenuation;
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

	vec3 spec = vec3(0.0);
    vec3 scattered = vec3(0.0);
    vec3 reflected = vec3(0.0);

	for (int l = 0; l < MAXLIGHTS; ++l) {
        if (!lights[l].is_enabled) continue;

        vec3 light_dir = lights[l].position;
        float attenuation = 1.0;

        if (lights[l].is_local) {
            light_dir -= DataIn.o_pos;
            float distance = length(light_dir);
            light_dir = normalize(light_dir);

            attenuation = 1.0;

            if (lights[l].is_spot) {
                float spot_cos = dot(light_dir, -normalize(lights[l].direction));

                if (spot_cos < lights[l].spot_cutoff)
                    attenuation = 0.0;
                else
                    attenuation *= pow(spot_cos,
                                        lights[l].spot_exponent);
            }
        } else
            light_dir = normalize(light_dir);

        float itensity = max(dot(light_dir, DataIn.normal), 0.0);
        float specular = 0.0;

        if (itensity > 0.0) {
            vec3 view = normalize(-DataIn.o_pos);
            vec3 half_v = normalize(light_dir + view);
            float spec_angle = max(dot(DataIn.normal, half_v), 0.0);
            specular = pow(spec_angle, mat.shininess);
        }

        scattered += lights[l].color * mat.ambient.rgb * attenuation +
                lights[l].color * mat.diffuse.rgb * itensity * attenuation;

        reflected += specular * mat.specular.rgb * lights[l].color * attenuation;
    }
    vec3 res = min(mat.emissive.rgb + mat.diffuse.rgb * scattered + reflected, vec3(1.0));
	
	colorOut = vec4(res, 1.0);
}