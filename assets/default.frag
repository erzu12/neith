# version 330 core
out vec4 FragColor;

in vec3 pos;

void main() {
    float stripeX = floor(mod(pos.x, 2)); 
    float stripeY = floor(mod(pos.y, 2)); 
    float stripeZ = floor(mod(pos.z, 2)); 
    float checker = 0.3;
    if(stripeY == stripeX) {
        checker = 0.7;
    }
    if(stripeZ == 1.0) {
        checker = checker * -1.0 + 1;
    }
    checker *= 0.3;
	FragColor = vec4(checker, checker, checker, 1.0);
}
