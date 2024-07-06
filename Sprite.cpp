#include "Sprite.h"
#include <stb_image.h>
#include <iostream>

/* Construtor da Classe Sprite
Recebe Shader, TextureID, posição, escala e rotação como parâmetros
Chama as funções setupGeometry() e updateModeMatrix() por padrão
*/

Sprite::Sprite(Shader& shader, const std::string& texturePath, glm::vec3 position, glm::vec3 tilePosition, glm::vec3 scale, float rotation)
    : shader(shader), position(position), tilePosition(tilePosition), scale(scale), rotation(rotation),
    timeAccumulator(0.0f), currentFrameX(0), currentFrameY(0) {
    textureID = loadTexture(texturePath);
    setupGeometry();
    updateModelMatrix();
}

// Construtor de movimento
Sprite::Sprite(Sprite&& other) noexcept
    : shader(other.shader), textureID(other.textureID), position(other.position), tilePosition(other.tilePosition),
    scale(other.scale), rotation(other.rotation), VAO(other.VAO), VBO(other.VBO), modelMatrix(other.modelMatrix) {
    other.textureID = 0;
    other.VAO = 0;
    other.VBO = 0;
}

// Copia as propriedades de um Sprite para o outro
Sprite& Sprite::operator=(Sprite&& other) noexcept {
    if (this != &other) {
        shader = other.shader;
        textureID = other.textureID;
        position = other.position;
        tilePosition = other.tilePosition;
        scale = other.scale;
        rotation = other.rotation;
        VAO = other.VAO;
        VBO = other.VBO;
        modelMatrix = other.modelMatrix;

        other.textureID = 0;
        other.VAO = 0;
        other.VBO = 0;
    }
    return *this;
}

// Função para atualizar o sprite
// Chama a função updateModelMatrix para recalcular a matriz de modelo
void Sprite::updateSprite() {
    updateModelMatrix();
}

// Função para renderizar o sprite
// Configura o shader, textura, e desenha o sprite na tela
void Sprite::draw() const {
    shader.Use();
    shader.setMat4("model", const_cast<float*>(glm::value_ptr(modelMatrix)));  // Envia a matriz de modelo ao shader
    glActiveTexture(GL_TEXTURE0);                                              // Ativa a unidade de textura 0
    glBindTexture(GL_TEXTURE_2D, textureID);                                   // Vincula a textura ao alvo de textura 2D
    glBindVertexArray(VAO);                                                    // Vincula o Vertex Array Object (VAO)
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);                                       // Desenha os vértices do sprite como um triângulo fan
    glBindVertexArray(0);                                                      // Desassocia o VAO
}


// Função para atualizar a textura de forma a animar o Sprite
void Sprite::updateTextureCoordsAnimated(int columns, int rows, float deltaTime, int startFrameX, int startFrameY, int endFrameX, int endFrameY) {
    // Calculate the total number of frames in the animation
    int totalFrames = (endFrameY - startFrameY) * columns + (endFrameX - startFrameX) + 1;

    // Update the time accumulator based on delta time
    timeAccumulator += deltaTime;

    // Update the frame if the time accumulator reaches the limit
    if (timeAccumulator >= 1.0f / totalFrames) {
        timeAccumulator = 0.0f; // Reset the time accumulator

        // Advance to the next frame
        currentFrameX++;
        if (currentFrameX > endFrameX) {
            currentFrameX = startFrameX;
            currentFrameY++;
            if (currentFrameY > endFrameY) {
                currentFrameY = startFrameY;
            }
        }
    }

    // Calculate texture offsets (s and t)
    float ds = 1.0f / columns;
    float dt = 1.0f / rows;

    // Calculate the position of the current frame on the spritesheet
    float offsetS = currentFrameX * ds;
    float offsetT = currentFrameY * dt;

    GLfloat vertices[] = {
        //      Coordinates       /       Colors        /    TexCoord
            -0.5f, -0.5f, 0.0f,      1.0f, 1.0f, 1.0f,       offsetS, offsetT,             // Lower left corner
             0.5f, -0.5f, 0.0f,      1.0f, 1.0f, 1.0f,       offsetS + ds, offsetT,        // Lower right corner
             0.5f,  0.5f, 0.0f,      1.0f, 1.0f, 1.0f,       offsetS + ds, offsetT + dt,   // Upper right corner
            -0.5f,  0.5f, 0.0f,      1.0f, 1.0f, 1.0f,       offsetS, offsetT + dt         // Upper left corner
    };

    // Update the vertex buffer with the new data
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// Função para atualizar a textura 
void Sprite::updateTextureCoordsStatic(int columns, int rows, int frameX, int frameY) {
    // Calculate texture offsets (s and t)
    float ds = 1.0f / columns;
    float dt = 1.0f / rows;

    // Calculate the position of the current frame on the spritesheet
    float offsetS = frameX * ds;
    float offsetT = frameY * dt;

    GLfloat vertices[] = {
        //      Coordinates       /       Colors        /    TexCoord
            -0.5f, -0.5f, 0.0f,      1.0f, 1.0f, 1.0f,       offsetS, offsetT,             // Lower left corner
             0.5f, -0.5f, 0.0f,      1.0f, 1.0f, 1.0f,       offsetS + ds, offsetT,        // Lower right corner
             0.5f,  0.5f, 0.0f,      1.0f, 1.0f, 1.0f,       offsetS + ds, offsetT + dt,   // Upper right corner
            -0.5f,  0.5f, 0.0f,      1.0f, 1.0f, 1.0f,       offsetS, offsetT + dt         // Upper left corner
    };

    // Update the vertex buffer with the new data
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// Função para atualizar o sprite
// Calcula a matriz de modelo com base na posição, rotação e escala do sprite
void Sprite::updateModelMatrix() {
    modelMatrix = glm::mat4(1.0f);                                                                   // Inicializa a matriz de modelo como a matriz identidade
    modelMatrix = glm::translate(modelMatrix, position);                                             // Aplica a translação
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0.0, 0.0, 1.0));        // Aplica a rotação
    modelMatrix = glm::scale(modelMatrix, scale);                                                    // Aplica a escala
}

// Função para retornar a ModelMatrix
glm::mat4 Sprite::getModelMatrix() const {
    return modelMatrix;
}

// Função para retornar o ID da textura atribuída
GLuint Sprite::getTextureID() const {
    return textureID;
}

glm::vec3 Sprite::getPosition() const {
    return position;
}

glm::vec3 Sprite::getTilePosition() const {
    return tilePosition;
}

void Sprite::setTilePosition(glm::vec3 new_position) {
    this->tilePosition = new_position; 
}

// Função para VAO e VBO define os atributos de posição, cor e coordenadas da textura
// Define os atributos de posição, cor e coordenadas de textura dos vértices e os armazena em buffers
void Sprite::setupGeometry() {
    GLfloat vertices[] = {
        //      Coordenadas        /       Cores          /    TexCoord
            -0.5f, -0.5f, 0.0f,      1.0f, 0.0f, 0.0f,       0.0f, 0.0f,     // Lower Left corner
             0.5f, -0.5f, 0.0f,      0.0f, 1.0f, 0.0f,       1.0f, 0.0f,     // Lower right corner
             0.5f, 0.5f, 0.0f,       0.0f, 0.0f, 1.0f,       1.0f, 1.0f,     // Upper right corner
            -0.5f, 0.5f, 0.0f,       1.0f, 1.0f, 0.0f,       0.0f, 1.0f      // Upper left corner
    };

    glGenBuffers(1, &VBO);                                                              // Gera um buffer de vértices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);                                                 // Associa o buffer ao alvo de buffer de array
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);          // Envia os dados de vértices para o buffer

    glGenVertexArrays(1, &VAO);                                                         // Gera um Vertex Array Object (VAO)
    glBindVertexArray(VAO);                                                             // Vincula o VAO

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);   // Posição dos vértices
    glEnableVertexAttribArray(0);                                                       // Habilita o atributo de posição

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));   // Cores dos vértices
    glEnableVertexAttribArray(1);                                                                           // Habilita o atributo de cor

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));   // Coordenadas de textura
    glEnableVertexAttribArray(2);                                                       // Habilita o atributo de coordenadas de textura

    glBindBuffer(GL_ARRAY_BUFFER, 0);                                                   // Desassocia o buffer de vértices
    glBindVertexArray(0);                                                               // Desassocia o VAO
}

// Função para carregar a textura e retornar o ID da textura
GLuint Sprite::loadTexture(const std::string& texturePath) {
    GLuint texID;

    // Gera o identificador da textura na memória
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    // Configuração do parâmetro WRAPPING nas coords s e t
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Configuração do parâmetro FILTERING na minificação e magnificação da textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Atributos básicos da imagem, largura, altura e número de canais de cores
    int width, height, nrChannels;
    // Habilita um flipagem vertical no carregamento da imagem
    stbi_set_flip_vertically_on_load(true);
    // Imagem
    unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);

    if (data) {
        if (nrChannels == 3) { //jpg, bmp
            // Texturiza a imagem com o RGB
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else { //png
         // Texturiza a imagem com o RGBA
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
        // Permite o PNG mesclar com o fundo caso tenha fundo nulo
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else {
        std::cerr << "Failed to load texture" << std::endl;
    }

    // Libera os dados da imagem
    stbi_image_free(data);
    // Desvincula a textura
    glBindTexture(GL_TEXTURE_2D, 0);

    return texID;
}