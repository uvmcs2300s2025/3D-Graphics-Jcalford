#include "cube.h"
#include "../util/debug.h"
#include <cassert>

Cube::Cube(Shader &shader, glm::vec3 pos, glm::vec3 size, vector<color> colors) {
    this->shader = shader;
    this->pos = pos;
    this->size = size;
    this->colors = colors;

    degreeX = degreeY = degreeZ = 0.0f;

    this->initVectors();
    this->initVAO();
    this->initVBO();
    this->initEBO();
}

Cube::~Cube() {
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Cube::setUniforms(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection) const {
    glm::mat4 modelMatrix = model;
    // The model matrix is used to transform the vertices of the shape in relation to the world space.
    modelMatrix = translate(modelMatrix, vec3(pos));
    // Rotate the cube
    modelMatrix = glm::rotate(modelMatrix, degreeX, glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, degreeY, glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, degreeZ, glm::vec3(0.0f, 0.0f, 1.0f));

    // The size of the shape is scaled by the model matrix to make the shape larger or smaller.
    modelMatrix = scale(modelMatrix, vec3(size));
    this->shader.setMatrix4("model", modelMatrix);
    this->shader.setMatrix4("view", view);
    this->shader.setMatrix4("projection", projection);
}

void Cube::rotateX(float delta) {
    degreeX += delta;
}

void Cube::rotateY(float delta) {
    degreeY += delta;
}

void Cube::rotateZ(float delta) {
    degreeZ += delta;
}

void Cube::changeSize(float delta) {
    size[0] += delta;
    size[1] += delta;
    size[2] += delta;
}

void Cube::draw(const mat4& model, const mat4& view, const mat4& projection) const {
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Cube::initVectors() {
    assert(colors.size() == 8);
    this->vertices.insert(this->vertices.end(), {
            // Front face:
             0.5f,  0.5f,  0.5f, colors[0].red, colors[0].green, colors[0].blue, // Top right
            -0.5f,  0.5f,  0.5f, colors[1].red, colors[1].green, colors[1].blue, // Top left
             0.5f, -0.5f,  0.5f, colors[2].red, colors[2].green, colors[2].blue, // Bottom right
            -0.5f, -0.5f,  0.5f, colors[3].red, colors[3].green, colors[3].blue, // Bottom left
            // TODO: complete this method by adding the four vertices that make the back side of the cube
            //       color them with colors[4] through colors[7]
            // Back face:
            // Bottom left
            // Bottom right
            // Top right
            // Top left
    });

    this->indices.insert(this->indices.end(), {
            // The indices reference the vertices above.
            // Each face is drawn from two triangles,
            //   and each triangle is drawn from three vertices.
            // Front face
            0, 1, 2,
            1, 2, 3,
            // TODO: complete the other five faces
            // Right face
            // Back face
            // Left face
            // Bottom face
            // Top face
    });
}

void Cube::initVAO() {
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);
}

void Cube::initVBO() {
    glGenBuffers(1, &this->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // Color attribute (3 floats)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Cube::initEBO() {
    glGenBuffers(1, &this->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
}
