# version 330 core
out vec4 FragColor;

in vec3 pos;

void main() {
    float stripeX = floor(mod(pos.x + 0.0001, 2));
    float stripeY = floor(mod(pos.y + 0.0001, 2));
    float stripeZ = floor(mod(pos.z + 0.0001, 2));
    float checker = 0.3;
    if(stripeY == stripeX) {
        checker = 0.7;
    }
    if(stripeZ == 1.0) {
        checker = checker * -1.0 + 1;
    }
    checker *= 0.3;
	FragColor = vec4(pos.y*0.1, pos.y*0.1, pos.y*0.1, 1.0);
}
