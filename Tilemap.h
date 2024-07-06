#pragma once
#ifndef TILEMAP_H
#define TILEMAP_H

#include "Sprite.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// Classe que representa o tilemap
class Tilemap {
public:
    // Construtor que inicializa o tilemap com o shader, caminho do arquivo de configura��o e dimens�es da tela
    Tilemap(Shader& shader, const std::string& configPath, float screenWidth, float screenHeight);

    // M�todos para obter o n�mero de linhas e colunas de tiles
    int getTileRows() const;
    int getTileColumns() const;

    // M�todos para obter a largura e altura da tela
    float getScreenWidth() const;
    float getScreenHeight() const;

    // M�todo para desenhar os tiles
    void drawTiles() const;

    // M�todo para verificar se um tile � caminh�vel
    bool isWalkable(int x, int y) const;

    // M�todo para obter o deslocamento dos tiles
    glm::vec2 getOffset() const;

    // M�todo para encontrar um sprite pela posi��o do tile
    Sprite* findSpriteByTilePosition(const glm::vec3& tilePosition);

    // M�todo para mudar a textura de um tile
    void changeTileTexture(int x, int y, int newTextureIndex);

    // M�todo para imprimir os tiles (usado para depura��o)
    void printTiles() const;

private:
    // M�todo para carregar o mapa a partir de um arquivo de configura��o
    void loadMap(const std::string& configPath);

    // M�todo para carregar uma textura a partir de um caminho de arquivo
    GLuint loadTexture(const std::string& texturePath);

    // M�todo para ordenar os tiles pela posi��o
    void sortTilesByPosition();

    Shader& shader;                             // Refer�ncia ao shader
    std::vector<Sprite> tiles;                  // Vetor de sprites dos tiles
    std::vector<std::vector<int>> mapData;      // Dados do mapa
    std::vector<int> nonWalkableTextures;       // Vetor de texturas n�o caminh�veis
    int mapWidth, mapHeight;                    // Largura e altura do mapa
    int tileCount;                              // Contagem de tiles
    int tileRows, tileColumns;                  // N�mero de linhas e colunas de tiles
    float screenWidth, screenHeight;            // Largura e altura da tela
};

#endif
