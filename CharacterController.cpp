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

// M�todo para verificar se o personagem est� em movimento
bool CharacterController::getMoving() {
    return moving;
}

// M�todos para mover o personagem em diferentes dire��es
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

// M�todo para mover o personagem se a posi��o for caminh�vel
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

// M�todo para atualizar a posi��o do personagem
void CharacterController::update(float deltaTime) {
    if (moving) {
        // Determina a dire��o para mover com base na posi��o alvo
        glm::vec3 direction = glm::normalize(targetPosition - position);

        // Move o personagem na dire��o por uma velocidade fixa escalada por deltaTime
        float speed = 150.0f; // Ajuste este valor para controlar a velocidade de movimento
        position += direction * speed * deltaTime;

        // Verifica se o personagem alcan�ou a posi��o alvo
        if (glm::distance(position, targetPosition) < 1.5f) {
            position = targetPosition; // Ajusta para a posi��o alvo
            moving = false;
        }

        // Atualiza o sprite com a nova posi��o
        updateSprite();
    }
}

// M�todo para atualizar a posi��o da c�mera
glm::vec3 CharacterController::updateCameraPosition(float deltaTime, glm::vec3 cameraPos, GLuint width, GLuint height) {
    // Define a posi��o desejada da c�mera (centralizada no personagem)
    glm::vec3 desiredCameraPos = position - glm::vec3(width / 2, height / 2, 0.0f);

    // Interpola a posi��o da c�mera em dire��o � posi��o desejada
    float smoothingFactor = 5.0f; // Ajuste este valor para controlar a suavidade
    cameraPos += (desiredCameraPos - cameraPos) * smoothingFactor * deltaTime;

    return cameraPos;
}

// M�todo para atualizar a dire��o do personagem
void CharacterController::updateDirection(bool shouldFlipX, float newRotation) {
    if (flipedX != shouldFlipX) {
        scale.x = -scale.x;
        flipedX = shouldFlipX;
    }
    this->rotation = newRotation;
}
