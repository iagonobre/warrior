# Guardians of the Core

## Descrição do Jogo

**Guardians of the Core** é um jogo de sobrevivência onde você controla um herói encarregado de defender uma base contra ondas de inimigos. O objetivo é manter a base intacta por 2 minutos enquanto enfrenta inimigos cada vez mais numerosos.

## Controles

- **Movimentação do Herói**: Clique com o botão direito do mouse para mover o herói.
- **Disparar Projéteis**: Pressione a tecla `Q` para atirar.
- **Pausar Jogo**: Pressione a tecla `P` para pausar o jogo.
- **Objetivo**: Sobreviver por 2 minutos e proteger a base.

### Recursos Implementados

1. **Herói:**

   - O herói começa com 100 de vida.
   - Ele possui uma munição inicial de 100 projéteis.
   - A cada disparo, o herói consome 1 munição.
   - Quando o herói é atingido por um projétil inimigo, ele perde 1 ponto de vida.
   - O herói pode coletar caixas de munição dropadas pelos inimigos abatidos, que restauram 10 munições.

2. **Base:**

   - A base começa com 100% de vida.
   - A cada impacto de um projétil inimigo, a base perde 1 ponto de vida.
   - Se um inimigo conseguir entrar na base, ela perde 5 pontos de vida.
   - A base regenera 1 ponto de vida a cada 5 segundos sem ser atingida.

3. **Inimigos:**

   - Os inimigos aparecem aleatoriamente nas bordas da tela e se movimentam em direção à base.
   - Um único disparo do herói elimina o inimigo.
   - Ao serem destruídos, os inimigos dropam caixas de munição (se a munição do herói estiver abaixo de 100).

4. **Caixas de Munição:**

   - As caixas de munição fornecem 10 projéteis ao herói.
   - Elas desaparecem após 5 segundos, caso não sejam coletadas.

5. **Objetivo:**
   - O objetivo é proteger a base por 2 minutos. Se a base chegar a 0% de vida ou o herói for derrotado, o jogo termina.

## Conceitos Utilizados

- **Herança e Polimorfismo:**
  Utilizados para implementar os inimigos e projéteis com comportamento diferenciado.

- **Sistema de Tempo:**
  O jogo utiliza o sistema de relógios da SFML para controlar spawn de inimigos, disparos e regeneração da base.

- **Detecção de Colisão:**
  A colisão entre os projéteis, inimigos e o herói/base é implementada com base nos limites (bounding boxes) dos objetos.

## Instruções de Compilação

O projeto foi desenvolvido usando a biblioteca **SFML** e o compilador **MinGW** no ambiente **Visual Studio Code**.

### Pré-requisitos

- **SFML**: Certifique-se de que a biblioteca **SFML** esteja instalada.
- **MinGW**: Compilador **g++** do **MinGW** deve estar configurado corretamente no sistema.

### Instruções para Compilar

1. **Instalar SFML e MinGW:**

   - Baixe e instale o **MinGW** em `C:/msys64/mingw64/` e adicione-o ao seu PATH.
   - Baixe e instale a biblioteca **SFML**.

2. **Configuração do VS Code:**
   O projeto está configurado para ser compilado com as seguintes configurações:

   - O arquivo `tasks.json` define como compilar o jogo:
     ```json
     {
       "command": "C:\\msys64\\mingw64\\bin\\g++.exe",
       "args": [
         "-g",
         "-std=c++20",
         "${workspaceFolder}\\main.cpp",
         "${workspaceFolder}\\src\\*.cpp",
         "-o",
         "${workspaceFolder}\\build\\main.exe",
         "-lsfml-system",
         "-lsfml-window",
         "-lsfml-graphics"
       ]
     }
     ```

3. **Compilação:**

   - Abra o projeto no Visual Studio Code.
   - Execute o atalho de **build** (`Ctrl+Shift+B` no Windows). Isso compilará o jogo gerando um arquivo executável na pasta `build`.

4. **Execução:**
   - Para executar o jogo, abra o terminal integrado no VS Code e use o comando:
     ```
     ./build/main.exe
     ```
