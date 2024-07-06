#ifndef CHARACTERCONTROLLER_H
#define CHARACTERCONTROLLER_H

#include <glm/glm.hpp>
#include <vector>
#include "Sprite.h"
#include "Tilemap.h"

// Classe CharacterController que herda de Sprite
class CharacterController : public Sprite {
public:
    // Construtor
    CharacterController(Shader& shader, const std::string& texturePath, glm::vec3 position, glm::vec3 tilePosition, glm::vec3 size, float rotate, Tilemap& tilemap);

    // M�todos para mover o personagem em diferentes dire��es
    void moveNorth(double time_delta);
    void moveSouth(double time_delta);
    void moveWest(double time_delta);
    void moveEast(double time_delta);
    void moveNorthWest(double time_delta);
    void moveNorthEast(double time_delta);
    void moveSouthWest(double time_delta);
    void moveSouthEast(double time_delta);

    // M�todo para verificar se o personagem est� em movimento
    bool getMoving();

    // M�todo para atualizar a posi��o do personagem
    void update(float deltaTime);

    // M�todo para atualizar a posi��o da c�mera
    glm::vec3 updateCameraPosition(float deltaTime, glm::vec3 cameraPos, GLuint width, GLuint height);

private:
    Tilemap& tilemap;                   // Refer�ncia ao tilemap
    float tileWidth;                    // Largura do tile
    float tileHeight;                   // Altura do tile
    glm::ivec2 targetTile;              // Tile alvo
    glm::vec3 targetPosition;           // Posi��o alvo
    bool flipedX;                       // Indicador se o sprite est� espelhado horizontalmente
    bool moving;                        // Indicador se o personagem est� se movendo
    float offsetX;                      // Deslocamento X
    float offsetY;                      // Deslocamento Y

    // M�todo para mover o personagem se a posi��o for caminh�vel
    void moveIfWalkable(int x, int y);

    // M�todo para atualizar a dire��o do personagem
    void updateDirection(bool shouldFlipX, float newRotation);
};

#endif
