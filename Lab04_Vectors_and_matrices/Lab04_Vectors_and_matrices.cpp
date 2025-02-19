#include <iostream>
// Include the glm library
#include <glm/glm.hpp> 
#include <glm/gtx/io.hpp>

#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>

int main() {
    
    // Vectors
    printf("Vectors and Matrices\n");
    printf("--------------------------------------------------\n");

    // Defining Vectors
    glm::vec3 a, b;
    a = glm::vec3(3.0f, 0.0f, 4.0f);
    b[0] = 1.3f, b[1] = 2.0f, b[2] = 3.0f;

    printf("Defining vectors:\n");
    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;

    return 0;
}
