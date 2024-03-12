uniform sampler2D generation;
uniform vec2 resolution;
uniform vec3 void_color;
uniform vec3 virus_color;

vec4 getNeighborColor(float x, float y) {
    return texture2D(generation, gl_TexCoord[0].xy + vec2(x, y) / resolution);
}
/*
int rgb2int(vec4 color) {
  int r = int(color.r * 255.0);
  int g = int(color.g * 255.0);
  int b = int(color.b * 255.0);

  return r * 65536 + g * 256 + b;
}

vec3 int2rgb(int color){
    int r = (color >> 16) & 0xFF;
    int g = (color >> 8) & 0xFF;
    int b = color & 0xFF;

    return vec3(float(r) / 255.0, float(g) / 255.0, float(b) / 255.0);
}*/

void main() {
    vec4 center = texture2D(generation, gl_TexCoord[0].xy);
    if (gl_FragCoord.x <= 1.0 || gl_FragCoord.x >= resolution.x - 1.0 ||
        gl_FragCoord.y <= 1.0 || gl_FragCoord.y >= resolution.y - 1.0) {
        gl_FragColor = vec4(void_color, 1.0);
    } 
    else {

        /*
        6 5 4  6 1 4
        7 c 3  3 c 7
        0 1 2  0 5 2
        */

        vec4 nbs[8];
        nbs[0] = getNeighborColor(-1.0, 1.0);
        nbs[1] = getNeighborColor(0.0, -1.0);
        nbs[2] = getNeighborColor(1.0, 1.0);
        nbs[3] = getNeighborColor(-1.0, 0.0);
        nbs[4] = getNeighborColor(1.0, -1.0);
        nbs[5] = getNeighborColor(0.0, 1.0);
        nbs[6] = getNeighborColor(-1.0, -1.0);
        nbs[7] = getNeighborColor(1.0, 0.0);

        int virus = 0;
        int organic = 0;
        int maxCount = 0;
        int minCount = 8;

        vec3 dominant = void_color;
        bool on_battle = false;

        for (int i = 0; i < 8; ++i) {
            if (nbs[i].rgb == virus_color)
                ++virus;
            
            else if (nbs[i].rgb != void_color)
                ++organic;
            
            else
                continue;

            int count = 0;
            for (int j = 0; j < 8; ++j) {
                if (i != j && nbs[i] == nbs[j]) {
                    ++count;
                }
            }
            if (count > maxCount) {
                maxCount = count;
                if (dominant != void_color && dominant != nbs[i].rgb){
                    on_battle = true;
                }
                dominant = nbs[i].rgb;
            }
            if (count < minCount){
                minCount = count;
            }
        }

        vec4 updated = center;

        //Special case: Infection

        bool center_organic = center.rgb != void_color && center.rgb != virus_color;

        if (virus > 1){
            dominant = virus_color;
        }

        bool mutation = center_organic && virus == 0 &&
                        (maxCount >= 2 && maxCount <= 5 && 
                        (nbs[1].rgb == void_color ^ nbs[5].rgb == void_color ^
                         nbs[7].rgb == void_color ^ nbs[3].rgb == void_color)) &&
                         on_battle;

        if (mutation){
            updated = vec4(virus_color, 1.0);
        }

        else if (center.rgb == virus_color && organic == 0) {
            updated = vec4(void_color, 1.0);
        } 

        else {
            if (virus > 0 && center_organic){
                updated = vec4(virus_color, 1.0);
            }
            else if (organic == 3){
                updated = vec4(dominant, 1.0);
            }
            else if (organic == 2){
                if (center.rgb == void_color || center.rgb == virus_color)
                    dominant = void_color;
                updated = vec4(dominant, 1.0);
            }
            else{
                if (dominant != virus_color)
                    updated = vec4(void_color, 1.0);
                else
                    updated = center;
            }
        }

        gl_FragColor = updated;
    }
}
