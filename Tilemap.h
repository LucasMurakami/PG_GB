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
    // Construtor que inicializa o tilemap com o shader, caminho do arquivo de configuração e dimensões da tela
    Tilemap(Shader& shader, const std::string& configPath, float screenWidth, float screenHeight);

    // Métodos para obter o número de linhas e colunas de tiles
    int getTileRows() const;
    int getTileColumns() const;

    // Métodos para obter a largura e altura da tela
    float getScreenWidth() const;
    float getScreenHeight() const;

    // Método para desenhar os tiles
    void drawTiles() const;

    // Método para verificar se um tile é caminhável
    bool isWalkable(int x, int y) const;

    // Método para obter o deslocamento dos tiles
    glm::vec2 getOffset() const;

    // Método para encontrar um sprite pela posição do tile
    Sprite* findSpriteByTilePosition(const glm::vec3& tilePosition);

    // Método para mudar a textura de um tile
    void changeTileTexture(int x, int y, int newTextureIndex);

    // Método para imprimir os tiles (usado para depuração)
    void printTiles() const;

private:
    // Método para carregar o mapa a partir de um arquivo de configuração
    void loadMap(const std::string& configPath);

    // Método para carregar uma textura a partir de um caminho de arquivo
    GLuint loadTexture(const std::string& texturePath);

    // Método para ordenar os tiles pela posição
    void sortTilesByPosition();

    Shader& shader;                             // Referência ao shader
    std::vector<Sprite> tiles;                  // Vetor de sprites dos tiles
    std::vector<std::vector<int>> mapData;      // Dados do mapa
    std::vector<int> nonWalkableTextures;       // Vetor de texturas não caminháveis
    int mapWidth, mapHeight;                    // Largura e altura do mapa
    int tileCount;                              // Contagem de tiles
    int tileRows, tileColumns;                  // Número de linhas e colunas de tiles
    float screenWidth, screenHeight;            // Largura e altura da tela
};

#endif
