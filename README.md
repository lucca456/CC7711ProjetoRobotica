# CC7711ProjetoRobotica
Este programa controla um robô na plataforma de simulação WeBots. O robô em questão é um E-Puck, que possui sensores de proximidade e LEDs, e tem a capacidade de se mover pelo ambiente simulado. O objetivo do programa é fazer o robô procurar uma caixa móvel no ambiente, empurrá-la, e acender seus LEDs quando estiver empurrando a caixa.

## Como funciona
O programa utiliza os sensores de proximidade do robô para detectar a presença de obstáculos e a caixa móvel. O robô é programado para mover-se pelo ambiente e virar à esquerda quando detecta um obstáculo à sua frente. Se detectar um obstáculo à direita, virará à esquerda, e vice-versa.

O programa utiliza a interface de supervisor do WeBots para obter informações sobre a posição do robô e da caixa móvel. Quando o robô está perto o suficiente da caixa para empurrá-la, o programa considera que ele está "empurrando a caixa".

Quando o robô começa a empurrar a caixa, todos os seus LEDs são acesos. Eles permanecem acesos enquanto o robô estiver empurrando a caixa. Se a caixa for devolvida à sua posição original, os LEDs são desligados.

## Implementação
O programa é escrito em C e utiliza as bibliotecas do WeBots para interagir com o robô e o ambiente de simulação. Ele define constantes para o número de sensores de proximidade e LEDs do robô, bem como a velocidade máxima do robô.

O programa inicia configurando o robô e habilitando seus sensores e LEDs. Em seguida, entra em um loop onde lê os valores dos sensores de proximidade e ajusta a direção do robô com base nesses valores. Utiliza as funções de supervisor para verificar a posição do robô e da caixa e determinar se o robô está empurrando a caixa.

Se o robô estiver empurrando a caixa, o programa acenderá os LEDs do robô. Se a caixa voltar à sua posição original, o programa desligará os LEDs. O loop continua até que a simulação seja terminada.

## Uso
Para usar este programa, você precisará do WeBots instalado no seu sistema. Você pode iniciar a simulação através da interface do WeBots e o programa controlará o robô automaticamente. Seu comportamento pode ser observado na visualização 3D fornecida pelo WeBots.

## Conclusão
Este programa fornece um exemplo simples de como controlar um robô no WeBots usando seus sensores e atuadores. Ele demonstra como o robô pode interagir com objetos no ambiente de simulação e responder a eles.

# Integrantes
Lucca Bonsi Guarreschi / 22.120.016-5  
João Vitor de Souza Lisboa / 22.120.082-7
