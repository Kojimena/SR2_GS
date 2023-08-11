#include <SDL2/SDL.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "fragment.h"
#include "uniform.h"
#include "print.h"
#include "color.h"
#include "shaders.h"
#include "load.h"
#include "triangle.h"
#include <array>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

std::array<std::array<float, WINDOW_WIDTH>, WINDOW_HEIGHT> zbuffer;
SDL_Renderer* renderer;

Uniform uniform;


// Declare a global clearColor of type Color
Color clearColor = {0, 0, 0}; // Initially set to black

// Declare a global currentColor of type Color
Color currentColor = {255, 255, 255}; // Initially set to white

// Function to clear the framebuffer with the clearColor
void clear() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    for (auto &row : zbuffer) {
        std::fill(row.begin(), row.end(), -99999.0f);
    }
}

// Function to set a specific pixel in the framebuffer to the currentColor
void point(Fragment f) {
    if (f.position.z > zbuffer[f.position.y][f.position.x]) {
        SDL_SetRenderDrawColor(renderer, f.color.r, f.color.g, f.color.b, f.color.a);
        SDL_RenderDrawPoint(renderer, f.position.x, f.position.y);
        zbuffer[f.position.y][f.position.x] = f.position.z;
    }
}

std::vector<std::vector<Vertex>> primitiveAssembly(
        const std::vector<Vertex>& transformedVertices
) {
    std::vector<std::vector<Vertex>> groupedVertices;

    for (int i = 0; i < transformedVertices.size(); i += 3) {
        std::vector<Vertex> vertexGroup;
        vertexGroup.push_back(transformedVertices[i]);
        vertexGroup.push_back(transformedVertices[i+1]);
        vertexGroup.push_back(transformedVertices[i+2]);

        groupedVertices.push_back(vertexGroup);
    }

    return groupedVertices;
}


void render(std::vector<glm::vec3> VBO) {
    // 1. Vertex Shader
    // vertex -> trasnformedVertices

    std::vector<Vertex> transformedVertices;

    for (int i = 0; i < VBO.size(); i+=2) {
        glm::vec3 v = VBO[i];
        glm::vec3 c = VBO[i+1];

        Vertex vertex = {v, Color(c.x, c.y, c.z)};
        Vertex transformedVertex = vertexShader(vertex, uniform);
        transformedVertices.push_back(transformedVertex);
    }


    // 2. Primitive Assembly
    // transformedVertices -> triangles
    std::vector<std::vector<Vertex>> triangles = primitiveAssembly(transformedVertices);

    // 3. Rasterize
    // triangles -> Fragments
    std::vector<Fragment> fragments;
    for (const std::vector<Vertex>& triangleVertices : triangles) {

        std::vector<Fragment> rasterizedTriangle = triangle(
            triangleVertices[0],
            triangleVertices[1],
            triangleVertices[2]
        );
        
        fragments.insert(
            fragments.end(),
            rasterizedTriangle.begin(),
            rasterizedTriangle.end()
        );
    }

    // 4. Fragment Shader
    // Fragments -> colors

    for (Fragment fragment : fragments) {
        point(fragmentShader(fragment));
    }
}

float a = 3.14f / 3.0f;
glm::mat4 createModelMatrix() {
    glm::mat4 transtation = glm::translate(glm::mat4(1), glm::vec3(1.0f, -0.9f, 0));
    glm::mat4 rotationy = glm::rotate(glm::mat4(1), glm::radians(a++), glm::vec3(0, 4, 0));
    glm::mat4 rotationx = glm::rotate(glm::mat4(1), glm::radians(3.14f), glm::vec3(1, 0, 0));
    glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(0.1f, 0.09f, 0.09f));
    return transtation * scale * rotationx * rotationy;
}


glm::mat4 createViewMatrix() {
    return glm::lookAt(
        // donde esta
        glm::vec3(0, 0, -5),
        // hacia adonde mira
        glm::vec3(0, 0, 0),
        // arriba
        glm::vec3(0, 1, 0)
    );
}


glm::mat4 createProjectionMatrix() {
    float fovInDegrees = 45.0f;
    float aspectRatio = WINDOW_WIDTH / WINDOW_HEIGHT;
    float nearClip = 0.1f;
    float farClip = 100.0f;

    return glm::perspective(glm::radians(fovInDegrees), aspectRatio, nearClip, farClip);
}

glm::mat4 createViewportMatrix() {
    glm::mat4 viewport = glm::mat4(1.0f);

    // Scale
    viewport = glm::scale(viewport, glm::vec3(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f, 0.5f));

    // Translate
    viewport = glm::translate(viewport, glm::vec3(1.0f, 1.0f, 0.5f));

    return viewport;
}

std::vector<glm::vec3> setupVertexArray(const std::vector<glm::vec3>& vertices, const std::vector<Face>& faces)
{
    std::vector<glm::vec3> vertexArray;

    // For each face
    for (const auto& face : faces)
    {
        // Get the first vertex of the face
        glm::vec3 firstVertex = vertices[face.vertexIndices[0] - 1]; // Subtract 1 from the index

        // For each additional vertex in the face
        for (size_t i = 2; i < face.vertexIndices.size(); ++i)
        {
            // Get the two vertices
            glm::vec3 vertex1 = vertices[face.vertexIndices[i - 1] - 1]; // Subtract 1 from the index
            glm::vec3 vertex2 = vertices[face.vertexIndices[i] - 1]; // Subtract 1 from the index

            // Add the vertices to the vertex array
            vertexArray.push_back(firstVertex);
            vertexArray.push_back(vertex1);
            vertexArray.push_back(vertex2);
        }
    }
    return vertexArray;
}



int main() {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window = SDL_CreateWindow("life", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    SDL_RenderSetLogicalSize(renderer, 400, 400);

    bool running = true;
    SDL_Event event;

    std::vector<glm::vec3> vertices;
    std::vector<Face> faces;
    if (!loadOBJ("/Users/jime/Downloads/navejime.obj", vertices, faces)) {
        std::cerr << "Failed to load model" << std::endl;
        return 1;
    }

    std::cout << "Loaded " << vertices.size() << " vertices and " << faces.size() << " faces." << std::endl;

    for (int i = 0; i < vertices.size(); i++) {
        std::cout << "v " << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << std::endl;
    }

    for (int i = 0; i < faces.size(); i++) {
        std::cout << "f " << faces[i].vertexIndices[0] << " " << faces[i].vertexIndices[1] << " " << faces[i].vertexIndices[2] << std::endl;
    }

    const std::vector<glm::vec3> &modelVertices = setupVertexArray(vertices, faces);


    while (running) {
        float angle = 0.0f;  // Mueve esta declaración fuera del bucle para que persista entre frames
        angle += 0.01f;

        uniform.model = glm::rotate(createModelMatrix(), angle, glm::vec3(0.0f, 1.0f, 0.0f));
        uniform.model = glm::translate(uniform.model, glm::vec3(0.0f, 0.0f, 0.0f));  // Ajusta estos valores según lo necesario

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event. type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        break;
                    case SDLK_DOWN:
                        break;
                    case SDLK_LEFT:
                        break;
                    case SDLK_RIGHT:
                        break;
                    case SDLK_s:
                        break;
                    case SDLK_a:
                        break;
                }
            }
        }

        uniform.model = createModelMatrix();
        uniform.view = createViewMatrix();
        uniform.projection = createProjectionMatrix();
        uniform.viewport = createViewportMatrix();


        clear();


        // Call our render function
        render(modelVertices);
        // point(10, 10, Color{255, 255, 255});


        // Present the frame buffer to the screen
        SDL_RenderPresent(renderer);

        // Delay to limit the frame rate
        SDL_Delay(1000 / 60);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}