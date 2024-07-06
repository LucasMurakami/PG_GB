#include "Tilemap.h"
#include <stb_image.h>
#include <algorithm>

const float TILE_SIZE = 128.0f; // Constante para indicar o tamanho do Sprite do Tile

// Construtor da classe Tilemap
Tilemap::Tilemap(Shader& shader, const std::string& configPath, float screenWidth, float screenHeight) : shader(shader), screenWidth(screenWidth), screenHeight(screenHeight) {
    loadMap(configPath);
}

// Fun��o para retornar a quantidade de colunas tile
int Tilemap::getTileColumns() const {
    return tileColumns;
}

// Fun��o para retornar a quantidade de linhas tile
int Tilemap::getTileRows() const {
    return tileRows;
}

// Fun��o para retornar a largura da tela
float Tilemap::getScreenWidth() const {
    return screenWidth;
}

// Fun��o para retornar a altura da tela 
float Tilemap::getScreenHeight() const {
    return screenHeight;
}

// Fun��o para buscar Sprite pela posi��o do tile
Sprite* Tilemap::findSpriteByTilePosition(const glm::vec3& tilePosition) {
    for (Sprite& tile : tiles) {
        if (tile.getTilePosition() == tilePosition) {
            return &tile;
        }
    }
    return nullptr;
}

// Fun��o que desenha os tiles na tela
void Tilemap::drawTiles() const {
 
    for (const Sprite& tile : tiles) {
        tile.draw();
    }
}

// Fun��o que checa se o tile permite o personagem andar nele
bool Tilemap::isWalkable(int x, int y) const {
    if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight) {
        return false;
    }
    int tileIndex = mapData[y][x];
    return std::find(nonWalkableTextures.begin(), nonWalkableTextures.end(), tileIndex) == nonWalkableTextures.end();
}

// Fun��o que carrega o tilemap
void Tilemap::loadMap(const std::string& configPath) {
    std::ifstream file(configPath);
    if (!file.is_open()) {
        std::cerr << "Failed to open configuration file" << std::endl;
        return;
    }

    std::string line;

    // L� o caminho da textura
    std::getline(file, line);
    std::string texturePath = line;

    // L� o a quantidade de texturas diferentes
    std::getline(file, line);
    std::stringstream ss(line);
    ss >> tileCount;

    // L� o valor da grid da spritesheet da textura
    std::getline(file, line);
    ss = std::stringstream(line);
    ss >> tileColumns >> tileRows;

    // L� o valor da grid do tilemap
    std::getline(file, line);
    ss = std::stringstream(line);
    ss >> mapWidth >> mapHeight;

    // L� o valor das texturas que n�o podem ser caminhadas
    std::getline(file, line);
    ss = std::stringstream(line);
    int textureNumber;
    while (ss >> textureNumber) {
        nonWalkableTextures.push_back(textureNumber);
    }

    // Calcula o offset
    glm::vec2 offset = getOffset();
    float offsetX = offset.x;
    float offsetY = offset.y;

    mapData.resize(mapHeight, std::vector<int>(mapWidth));
    for (int y = 0; y < mapHeight; ++y) {
        std::getline(file, line);
        ss = std::stringstream(line);
        for (int x = 0; x < mapWidth; ++x) {
            ss >> mapData[y][x];

            // Calcula a coordenada da textura 
            int tileIndex = mapData[y][x];
            int tileX = tileIndex % tileColumns;
            int tileY = tileIndex / tileColumns;

            // Ajusta a posi��o dos tiles para diamond view
            float isoX = (x - y) * (TILE_SIZE / 2.0f);
            float isoY = (x + y) * (TILE_SIZE / 4.0f);

            // Ajusta a posi��o com os offsets para centraliza��o
            glm::vec3 position(isoX + offsetX, isoY + offsetY, 0.0f);

            // Posi��o do tile no mundo
            glm::vec3 tilePosition(x, y, 0.0f);           

            // Cria o Sprite de tile
            Sprite tile(shader, texturePath, position, tilePosition, glm::vec3(TILE_SIZE, TILE_SIZE, 1.0f), 0.0f);
            tile.updateTextureCoordsStatic(tileColumns, tileRows, tileX, tileY);
            tiles.emplace_back(std::move(tile));
        }
    }
    // Organiza os tiles baseado no valor Y para renderiza��o 
    sortTilesByPosition();
}

// Fun��o para calcular o offset
glm::vec2 Tilemap::getOffset() const {
    // Calcula o centro da tela e o centro do tilemap para centralizar o mapa
    float screenCenterX = screenWidth / 2.0f;
    float screenCenterY = screenHeight / 2.0f;

    float tilemapCenterX = (mapWidth * TILE_SIZE) / 2.0f;
    float tilemapCenterY = (mapHeight * TILE_SIZE) / 2.0f;

    // Calcula o valor total da largue e altura do tilemap na tela
    float tilemapWidth = mapWidth * TILE_SIZE;
    float tilemapHeight = mapHeight * TILE_SIZE;

    // Calcula os offsets para centralizar o tilemap
    float offsetX = screenCenterX;
    float offsetY = screenCenterY - tilemapCenterY + 100.0f;

    return glm::vec2(offsetX, offsetY);
}

// Fun��o para verificar as informa��es dos Sprite tiles
void Tilemap::printTiles() const {
    for (const auto& tile : tiles) {
        glm::vec3 position = tile.getPosition();
        glm::vec3 tilePosition = tile.getTilePosition();
        std::cout << "Tile Position: (" << tilePosition.x << ", " << tilePosition.y << ", " << tilePosition.z << ")"
            << ", World Position: (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
    }
}

// Fun��o para organizar os tiles pela posi��o Y
void Tilemap::sortTilesByPosition() {
    std::sort(tiles.begin(), tiles.end(), [](const Sprite& a, const Sprite& b) {
        return a.getPosition().y > b.getPosition().y;
        });
}

// Fun��o para mudar a textura do tile
void Tilemap::changeTileTexture(int x, int y, int newTextureIndex) {
    mapData[y][x] = newTextureIndex;
    Sprite* tile = findSpriteByTilePosition(glm::vec3(x, y, 0.0f));
    if (tile) {
        int tileX = newTextureIndex % tileColumns;
        int tileY = newTextureIndex / tileColumns;
        tile->updateTextureCoordsStatic(tileColumns, tileRows, tileX, tileY);
    }
}

// Fun��o para carregar a textura e retornar o ID da textura
GLuint Tilemap::loadTexture(const std::string& texturePath) {
    GLuint texID;

    // Gera o identificador da textura na mem�ria
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    // Configura��o do par�metro WRAPPING nas coords s e t
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Configura��o do par�metro FILTERING na minifica��o e magnifica��o da textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Atributos b�sicos da imagem, largura, altura e n�mero de canais de cores
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