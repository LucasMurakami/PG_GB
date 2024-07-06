#include <iostream>
#include <string>
#include <assert.h>
#include <windows.h>

#include "Sprite.h"
#include "Tilemap.h"
#include "CharacterController.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// STB IMAGE
#include <stb_image.h>

using namespace std;

// Classe para manipulação dos shaders
#include "Shader.h"

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Funções auxiliares da lógica do jogo
void textureChanges(Tilemap& tilemap, int x, int y);
void potionTracker(int x, int y);
void winCondition(Tilemap& tilemap, int x, int y);

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 600;

// Câmera
glm::vec3 cameraPos; 

// Controlador
CharacterController* controller = nullptr;

// Potion Counter
bool potionCheck1 = false;
bool potionCheck2 = false;

// Tempo Delta
double time_now, time_old, time_delta;

// Função MAIN
int main()
{
    // Inicialização da GLFW
    glfwInit();

    // Criação da janela GLFW
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Jogo GB", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Fazendo o registro da função de callback para a janela GLFW
    glfwSetKeyCallback(window, key_callback);

    // GLAD: carrega todos os ponteiros de funções da OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
    }

    // Obtendo as informações de versão
    const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
    const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
    cout << "Renderer: " << renderer << endl;
    cout << "OpenGL version supported " << version << endl;

    // Compilando e buildando o programa de shader
    Shader shader("tex.vs", "tex.fs");

    // Ativando o buffer de textura 0 da opengl
    glActiveTexture(GL_TEXTURE0);

    // Ativa o shader
    shader.Use();

    // Matriz de projeção paralela ortográfica
    glm::mat4 projection = glm::ortho(0.0, static_cast<double>(WIDTH), 0.0, static_cast<double>(HEIGHT), -1.0, 1.0);

    // Enviando para o shader a matriz como uma var uniform
    shader.setMat4("projection", glm::value_ptr(projection));

    // Criação dos objetos a serem renderizados
    Tilemap tilemap(shader, "Assets/map.txt", WIDTH, HEIGHT);
    Sprite* potionLocationSprite1 = tilemap.findSpriteByTilePosition(glm::vec3(4,5,0));
    glm::vec3 potionLocationPosition1 = potionLocationSprite1->getPosition();
    glm::vec3 potionLocationTilePosition1 = potionLocationSprite1->getTilePosition();

    Sprite* potionLocationSprite2 = tilemap.findSpriteByTilePosition(glm::vec3(9, 10, 0));
    glm::vec3 potionLocationPosition2 = potionLocationSprite2->getPosition();
    glm::vec3 potionLocationTilePosition2 = potionLocationSprite2->getTilePosition();

    Sprite potion1(shader, "Assets/Util/PotionsSheet.png", glm::vec3(potionLocationPosition1.x, potionLocationPosition1.y + 45.0f, potionLocationPosition1.z), potionLocationTilePosition1, glm::vec3(50.0f, 50.0f, 0.0f), 0.0f);
    Sprite potion2(shader, "Assets/Util/PotionsSheet.png", glm::vec3(potionLocationPosition2.x, potionLocationPosition2.y + 45.0f, potionLocationPosition2.z), potionLocationTilePosition2, glm::vec3(50.0f, 50.0f, 0.0f), 0.0f);


    // Initial Position
    Sprite* initialSprite = tilemap.findSpriteByTilePosition(glm::vec3(14, 14, 0.0f));
    glm::vec3 initialPosition = initialSprite->getPosition();
    cameraPos = glm::vec3(initialPosition.x - WIDTH / 2, initialPosition.y - HEIGHT / 2, initialPosition.z);
    initialPosition.y += 85.0f;
    glm::vec3 initialTilePosition = initialSprite->getTilePosition();

    // Controller
    CharacterController character(shader, "Assets/Character/CharacterSheet_CharacterFront.png", initialPosition, initialTilePosition, glm::vec3(150.0f, 150.0f, 0.0f), 0.0f, tilemap);
    controller = &character;

    // Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height); // unidades de tela: pixel

    // Animation
    time_now = time_old = glfwGetTime();

    // Animation frame rate control
    double targetFrameRate = 60.0; // Frame Rate
    double targetFrameTime = 1.0 / targetFrameRate; // Tempo por frame

    // Câmera posicionamento
    glm::mat4 view = glm::translate(glm::mat4(1.0f), -cameraPos);

    // Loop da aplicação - "game loop"
    while (!glfwWindowShouldClose(window))
    {
        // Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
        glfwPollEvents();

        // Calcular o tempo delta
        time_now = glfwGetTime();
        time_delta = time_now - time_old;

        // Limpa o buffer de cor
        glClearColor(0.680, 0.9451, 0.9451, 1.0f); // cor de fundo
        glClear(GL_COLOR_BUFFER_BIT);

        // Câmera control
        view = glm::translate(glm::mat4(1.0f), -cameraPos);
        shader.setMat4("view", glm::value_ptr(view));

        // Game Logic
        glm::vec3 characterPosition = character.getTilePosition();
        int x = characterPosition.x;
        int y = characterPosition.y;

        // Atualiza a animação com base na frame rate
        if (time_delta >= targetFrameTime) {
            time_old = time_now;

            // Atualiza animação 
            character.updateTextureCoordsAnimated(5, 3, time_delta, 0, 0, 3, 0);
            potion1.updateTextureCoordsAnimated(2, 8, time_delta, 0, 7, 1, 7);
            potion2.updateTextureCoordsAnimated(2, 8, time_delta, 0, 6, 1, 6);

            // Atualiza o personagem na tela
            character.update(time_delta);
            cameraPos = character.updateCameraPosition(time_delta, cameraPos, WIDTH, HEIGHT);
            potion1.updateSprite();
            potion2.updateSprite();

            // Atualiza tilemap
            textureChanges(tilemap, x, y);

        }

        // Chamadas de desenho da cena
        tilemap.drawTiles();
       
        if (!potionCheck1) {
            potion1.draw();
        }

        if (!potionCheck2) {
            potion2.draw();
        }

        character.draw();

        // Troca os buffers da tela
        glfwSwapBuffers(window);
    }

    // Finaliza a execução da GLFW, limpando os recursos alocados por ela
    glfwTerminate();
    return 0;
}

// Função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    // Input do jogador para controlar o movimento do personagem
    if (!controller->getMoving()) {
        if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
            controller->moveNorth(time_delta);
        if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
            controller->moveSouth(time_delta);
        if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
            controller->moveWest(time_delta);
        if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
            controller->moveEast(time_delta);
        if (key == GLFW_KEY_E && (action == GLFW_PRESS || action == GLFW_REPEAT))
            controller->moveNorthEast(time_delta);
        if (key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_REPEAT))
            controller->moveNorthWest(time_delta);
        if (key == GLFW_KEY_C && (action == GLFW_PRESS || action == GLFW_REPEAT))
            controller->moveSouthWest(time_delta);
        if (key == GLFW_KEY_Z && (action == GLFW_PRESS || action == GLFW_REPEAT))
            controller->moveSouthEast(time_delta);
    }
}

// Função para atualizar 
void textureChanges(Tilemap& tilemap, int x, int y) {  

    if (x == 14 && y == 1) {
        tilemap.changeTileTexture(x - 1, y, 44);
    }

    // Potion 1 Floor
    if (x == 4 && y == 5) {
        tilemap.changeTileTexture(x , y , 133);
        tilemap.changeTileTexture(x - 1, y - 1 , 105);
        tilemap.changeTileTexture(x + 1, y + 1, 105);
        tilemap.changeTileTexture(x + 1 , y - 1, 105);
        tilemap.changeTileTexture(x - 1, y + 1, 105);

        tilemap.changeTileTexture(x - 1, y, 106);
        tilemap.changeTileTexture(x + 1, y, 106);
        tilemap.changeTileTexture(x, y + 1, 106);
        tilemap.changeTileTexture(x, y - 1, 106);

        potionTracker(x, y);
    }

    // Potion 2 Floor
    if (x == 9 && y == 10) {
        tilemap.changeTileTexture(x, y, 133);
        tilemap.changeTileTexture(x - 1, y - 1, 105);
        tilemap.changeTileTexture(x + 1, y + 1, 105);
        tilemap.changeTileTexture(x + 1, y - 1, 105);
        tilemap.changeTileTexture(x - 1, y + 1, 105);

        tilemap.changeTileTexture(x - 1, y, 106);
        tilemap.changeTileTexture(x + 1, y, 106);
        tilemap.changeTileTexture(x, y + 1, 106);
        tilemap.changeTileTexture(x, y - 1, 106);

        potionTracker(x, y);
    }

    winCondition(tilemap,x, y);
}

// Função para checar a contagem de poções
void potionTracker(int x, int y) {

    // Potion 1 
    if (x == 4 && y == 5) {
        potionCheck1 = true;
    }

    // Potion 2 
    if (x == 9 && y == 10) {
        potionCheck2 = true;
    }

}

// Função para checar se o jogador ganhou
void winCondition(Tilemap& tilemap, int x, int y) {

    if (potionCheck1 && potionCheck2) {
        tilemap.changeTileTexture(0, 0, 72);
        if (x == 0 && y == 0) {
            // Close the game window
            glfwSetWindowShouldClose(glfwGetCurrentContext(), GL_TRUE);
        }
    }
}