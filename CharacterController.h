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

    // Métodos para mover o personagem em diferentes direções
    void moveNorth(double time_delta);
    void moveSouth(double time_delta);
    void moveWest(double time_delta);
    void moveEast(double time_delta);
    void moveNorthWest(double time_delta);
    void moveNorthEast(double time_delta);
    void moveSouthWest(double time_delta);
    void moveSouthEast(double time_delta);

    // Método para verificar se o personagem está em movimento
    bool getMoving();

    // Método para atualizar a posição do personagem
    void update(float deltaTime);

    // Método para atualizar a posição da câmera
    glm::vec3 updateCameraPosition(float deltaTime, glm::vec3 cameraPos, GLuint width, GLuint height);

private:
    Tilemap& tilemap;                   // Referência ao tilemap
    float tileWidth;                    // Largura do tile
    float tileHeight;                   // Altura do tile
    glm::ivec2 targetTile;              // Tile alvo
    glm::vec3 targetPosition;           // Posição alvo
    bool flipedX;                       // Indicador se o sprite está espelhado horizontalmente
    bool moving;                        // Indicador se o personagem está se movendo
    float offsetX;                      // Deslocamento X
    float offsetY;                      // Deslocamento Y

    // Método para mover o personagem se a posição for caminhável
    void moveIfWalkable(int x, int y);

    // Método para atualizar a direção do personagem
    void updateDirection(bool shouldFlipX, float newRotation);
};

#endif
