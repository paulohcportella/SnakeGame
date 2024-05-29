# SnakeGame 🐍
Jogo Snake 2D desenvolvido em C utilizando a cli-lib para a disciplina de PIF.
Professores: Pamela Bezerra \\ Tiago Barros

O jogo Snake é um clássico dos videogames, originado na década de 1970 e popularizado em dispositivos móveis nos anos 1990. O objetivo é controlar uma cobra que cresce continuamente ao comer frutas, evitando colidir com as paredes do cenário ou com seu próprio corpo. Com o passar do tempo, a cobra se torna mais longa e o desafio aumenta.

# Recursos Principais:
- Movimento Contínuo: A cobra se move constantemente na direção determinada pelo jogador.
- Pontuação: Cada fruta consumida aumenta a pontuação do jogador.
- Colisão: O jogo termina se a cobra colidir com as paredes ou com seu próprio corpo.
- últimas resultados: Após o GameOver o jogador poderá colocar seu nome para registrar sua ultima pontuação obtida.

# 🕹️Como Jogar:
- Baixe os arquivos do github direto para o diretório da sua maquina e digite a linha de comando:
  gcc -o main main.c keyboard.c screen.c timer.c
  
- depois digite:
  ./main
  
- Utilize as teclas "W", "A", "S", "D") para controlar a direção da cobra.
- Usando a tecla "ESC" você pode sair do jogo. 
- Tente comer o máximo de frutas possível sem colidir com as paredes ou com a própria cobra.
- Cada fruta ("X") consumida aumenta sua pontuação.
- O jogo termina quando a cobra colide com as paredes ou com seu próprio corpo.

# 💻Conceitos Implementados:
- Estruturas (structs): Utilizadas para representar as coordenadas da cobra e dos objetos no jogo.
- Ponteiros: Empregados para alocação dinâmica de memória e manipulação da cobra.
- Alocação Dinâmica de Memória: Utilizada para criar a cobra com tamanho variável.
- Escrita e Leitura em Arquivo: As últimas pontuações dos jogadores são salvas e lidas de um arquivo.
- Divirta-se jogando o Snake e desafie-se a alcançar a maior pontuação possível!

# Desenvolvido por:
- Paulo Portella (phcp@cesar.school)
- João Guilherme Chaves (jgcl@cesar.school)
- Juliana Van Der Linden (jvdlm@cesar.school)
