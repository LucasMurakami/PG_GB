// CharacterController.cpp

#include "CharacterController.h"

const float TILE_SIZE = 128.0f;

// Construtor da classe CharacterController
CharacterController::CharacterController(Shader& shader, const std::string& texturePath, glm::vec3 position, glm::vec3 tilePosition, glm::vec3 size, float rotate, Tilemap& tilemap)
    : Sprite(shader, texturePath, position, tilePosition, size, rotate), tilemap(tilemap), moving(false)
{
    targetTile = tilePosition;
    targetPosition = position;
    offsetX = 0.0f;
    offsetY = 0.0f;
    flipedX = false;
}

// Método para verificar se o personagem está em movimento
bool CharacterController::getMoving() {
    return moving;
}

// Métodos para mover o personagem em diferentes direções
void CharacterController::moveNorth(double time_delta) {
    moveIfWalkable(targetTile.x, targetTile.y + 1);
    updateDirection(false, 0.0f);
}

void CharacterController::moveSouth(double time_delta) {
    moveIfWalkable(targetTile.x, targetTile.y - 1);
    updateDirection(true, -15.0f);
}

void CharacterController::moveWest(double time_delta) {
    moveIfWalkable(targetTile.x - 1, targetTile.y);
    updateDirection(false, 15.0f);
}

void CharacterController::moveEast(double time_delta) {
    moveIfWalkable(targetTile.x + 1, targetTile.y);
    updateDirection(true, 0.0f);
}

void CharacterController::moveNorthWest(double time_delta) {
    moveIfWalkable(targetTile.x - 1, targetTile.y + 1);
    updateDirection(false, 0.0f);
}

void CharacterController::moveNorthEast(double time_delta) {
    moveIfWalkable(targetTile.x + 1, targetTile.y + 1);
    updateDirection(false, 0.0f);
}

void CharacterController::moveSouthWest(double time_delta) {
    moveIfWalkable(targetTile.x - 1, targetTile.y - 1);
    updateDirection(false, 0.0f);
}

void CharacterController::moveSouthEast(double time_delta) {
    moveIfWalkable(targetTile.x + 1, targetTile.y - 1);
    updateDirection(true, 0.0f);
}

// Método para mover o personagem se a posição for caminhável
void CharacterController::moveIfWalkable(int x, int y) {
    if (tilemap.isWalkable(x, y)) {
        auto tile = tilemap.findSpriteByTilePosition(glm::vec3(x, y, 0.0f));
        if (tile) {
            targetTile = glm::ivec2(x, y);
            targetPosition = tile->getPosition();
            targetPosition.y += 85.0f; // altura do personagem

            tilePosition = tile->getTilePosition();
            moving = true;
        }
    }
}

// Método para atualizar a posição do personagem
void CharacterController::update(float deltaTime) {
    if (moving) {
        // Determina a direção para mover com base na posição alvo
        glm::vec3 direction = glm::normalize(targetPosition - position);

        // Move o personagem na direção por uma velocidade fixa escalada por deltaTime
        float speed = 150.0f; // Ajuste este valor para controlar a velocidade de movimento
        position += direction * speed * deltaTime;

        // Verifica se o personagem alcançou a posição alvo
        if (glm::distance(position, targetPosition) < 1.5f) {
            position = targetPosition; // Ajusta para a posição alvo
            moving = false;
        }

        // Atualiza o sprite com a nova posição
        updateSprite();
    }
}

// Método para atualizar a posição da câmera
glm::vec3 CharacterController::updateCameraPosition(float deltaTime, glm::vec3 cameraPos, GLuint width, GLuint height) {
    // Define a posição desejada da câmera (centralizada no personagem)
    glm::vec3 desiredCameraPos = position - glm::vec3(width / 2, height / 2, 0.0f);

    // Interpola a posição da câmera em direção à posição desejada
    float smoothingFactor = 5.0f; // Ajuste este valor para controlar a suavidade
    cameraPos += (desiredCameraPos - cameraPos) * smoothingFactor * deltaTime;

    return cameraPos;
}

// Método para atualizar a direção do personagem
void CharacterController::updateDirection(bool shouldFlipX, float newRotation) {
    if (flipedX != shouldFlipX) {
        scale.x = -scale.x;
        flipedX = shouldFlipX;
    }
    this->rotation = newRotation;
}
