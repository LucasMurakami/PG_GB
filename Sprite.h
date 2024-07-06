#ifndef SPRITE_H
#define SPRITE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

class Sprite { 

public:
    // Construtor: inicializa o sprite com o shader, ID da textura, posi��o, posi��o em 2.5D, escala e rota��o
    Sprite(Shader& shader, const std::string& texturePath, glm::vec3 position, glm::vec3 tilePosition, glm::vec3 scale, float rotation);

    // Deleta a c�pia do construtor e aloca para outro Sprite
    Sprite(const Sprite&) = delete;
    Sprite& operator=(const Sprite&) = delete;

    // Define o construtor de transfer�ncia e aloca para outro Sprite
    Sprite(Sprite&& other) noexcept;
    Sprite& operator=(Sprite&& other) noexcept;

    // Atualiza a matriz de modelo do sprite
    void updateSprite();

    // Renderiza o sprite na tela
    void draw() const;

    // Atualiza o estado da textura em anima��o
    void updateTextureCoordsAnimated(int columns, int rows, float deltaTime, int startFrameX, int startFrameY, int endFrameX, int endFrameY);

    // Atualiza o estado da textura est�tica
    void updateTextureCoordsStatic(int columns, int rows, int frameX, int frameY);

    // Retorna a matriz de modelo atual do sprite
    glm::mat4 getModelMatrix() const;

    // Retorna a posi��o do Sprite
    glm::vec3 getPosition() const; 

    // Retorna a posi��o do Sprite em rela��o aos tiles
    glm::vec3 getTilePosition() const;

    // Retorna o ID da textura associada ao sprite
    GLuint getTextureID() const;

    // Redefine a vari�vel tilePosition
    void setTilePosition(glm::vec3 new_position);

protected:
    // Vari�veis de renderiza��o do objeto Sprite
    Shader& shader;          // Refer�ncia ao shader usado pelo sprite
    GLuint textureID;        // ID da textura associada ao sprite
    std::string texturePath; // Store texture path for copying
    glm::vec3 position;      // Posi��o do sprite
    glm::vec3 tilePosition;  // Posi��o do sprite em rela��o aos tiles
    glm::vec3 scale;         // Escala do sprite
    float rotation;          // Rota��o do sprite
    GLuint VAO;              // Vertex Array Object do sprite
    GLuint VBO;              // Vertex Buffer Object do sprite
    glm::mat4 modelMatrix;   // Matriz de modelo do sprite

    float timeAccumulator;
    int currentFrameX;
    int currentFrameY;

    // Atualiza a matriz de modelo do sprite com base na posi��o, rota��o e escala
    void updateModelMatrix();

    // Configura a geometria do sprite definindo os atributos de v�rtices
    void setupGeometry();

    // Carrega a textura e retorna um ID da textura
    GLuint loadTexture(const std::string& texturePath);

};

#endif
