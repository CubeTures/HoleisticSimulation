#version 330

in vec3 fragPosition;
in vec3 fragNormal;

uniform vec3 viewPos;

#define MAX_LIGHTS        4
#define LIGHT_DIRECTIONAL 0
#define LIGHT_POINT       1

struct Light {
    int enabled;
    int type;
    vec3 position;
    vec3 target;
    vec4 color;
};

uniform Light lights[MAX_LIGHTS];

uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float s;

out vec4 finalColor;

void main()
{
    vec3 n = normalize(fragNormal);
    vec3 e = normalize(viewPos - fragPosition);

    vec3 color = ka;

    for (int i = 0; i < MAX_LIGHTS; i++) {
        if (lights[i].enabled == 0) continue;

        vec3 l;
        if (lights[i].type == LIGHT_DIRECTIONAL) {
            l = -normalize(lights[i].target - lights[i].position);
        } else {
            l = normalize(lights[i].position - fragPosition);
        }
        vec3 h = normalize(l + e);

        vec3 cd = kd * max(0.0, dot(n, l));
        vec3 cs = ks * pow(max(0.0, dot(n, h)), s);

        color += (cd + cs) * lights[i].color.rgb;
    }

    finalColor = vec4(color, 1.0);
}
