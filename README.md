# Projeto de Jogo Grau B

Este projeto é uma demonstração de jogo de tilemap isométrico simples, desenvolvido em C++ utilizando OpenGL, GLFW e outras bibliotecas.

## Requisitos

- Visual Studio 2019
- Bibliotecas:
  - OpenGL
  - GLFW
  - GLM
  - STB_IMAGE

## Instruções de Compilação

### Passo 1: Configurar o Visual Studio 2019

1. **Abrir o Projeto:**

   - Abra o Visual Studio 2019.
   - Vá para `Arquivo` > `Abrir` > `Projeto/Solução`.
   - Selecione a solução do projeto (arquivo `.sln`).

2. **Configurar Diretórios de Inclusão:**

   - Na `Solution Explorer`, clique com o botão direito no nome da solução e selecione `Propriedades`.
   - Vá para `Diretórios VC++` > `Diretórios de Inclusão`.
   - Adicione o caminho para a pasta `Include` das bibliotecas que você está utilizando. Por exemplo: `C:\path\to\your\libs\include`.

3. **Configurar Diretórios de Biblioteca:**

   - Ainda nas propriedades do projeto, vá para `Vinculador` > `Geral` > `Diretórios de Biblioteca Adicionais`.
   - Adicione o caminho para a pasta `lib` das bibliotecas que você está utilizando. Por exemplo: `C:\path\to\your\libs\lib`.

4. **Adicionar Dependências Adicionais:**
   - Vá para `Vinculador` > `Entrada` > `Dependências Adicionais`.
   - Adicione as seguintes dependências:
     - `opengl32.lib`
     - `glfw3.lib`
     - `glfw3.dll`

### Passo 2: Compilar e Executar o Projeto

1. **Selecionar Configuração de Build:**

   - No topo da janela do Visual Studio, selecione `Debug` ou `Release` e `x86`, conforme necessário.

2. **Compilar o Projeto:**

   - Pressione `Ctrl+Shift+B` para compilar a solução.
   - Verifique a `Output Window` para garantir que a compilação foi bem-sucedida.

3. **Executar o Projeto:**
   - Pressione `F5` para iniciar o projeto com depuração ou `Ctrl+F5` para iniciar sem depuração.
