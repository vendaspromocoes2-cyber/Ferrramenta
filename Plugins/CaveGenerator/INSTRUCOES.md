# Como Integrar e Usar o Plugin CaveGenerator (Fase 3 - Sistema de Salas)

Esta versão introduz o sistema de marcadores, permitindo que você crie salas de combate e outros pontos de interesse conectados aos túneis.

## Instalação

Se você já instalou as fases anteriores, apenas substitua os arquivos na pasta `Plugins/CaveGenerator` pelos novos e **recompile o projeto** no Visual Studio/Rider. Se for a primeira vez, siga os passos da Fase 1.

---

## Passo 4: Usando o Sistema de Marcadores de Sala

Abra o seu Blueprint `BP_CaveTunnel` e selecione o `Cave Generator Component`. No painel **Details**, abaixo da categoria `Tunnels`, você encontrará a nova categoria **`Rooms`**.

### 1. Adicionando uma Sala

1.  Selecione a propriedade **`Room Markers`**.
2.  Clique no ícone **"+"** para adicionar um novo elemento ao array. Cada elemento representa uma sala.
3.  Expanda o novo elemento (ex: `[0]`) para ver suas propriedades.

### 2. Configurando uma Sala

Cada marcador de sala (`Room Marker`) tem as seguintes propriedades:

*   **`Position` (Posição):**
    *   Este é o valor mais importante. Ele define **onde** a sala aparecerá ao longo do túnel.
    *   `0.0` é o início da spline.
    *   `1.0` é o fim da spline.
    *   Um valor de `0.5` posicionará a sala exatamente no meio do túnel.

*   **`Shape` (Formato):**
    *   Um menu dropdown onde você pode escolher o formato da sala.
    *   Atualmente, as opções são **`Circular`** e **`Rectangular`** (a implementação atual foca na circular, a retangular é visualmente similar por enquanto).

*   **`Size` (Tamanho):**
    *   Um vetor (X, Y, Z) que define as dimensões da sala. A interpretação depende do formato:
        *   **Para `Circular`:**
            *   `X`: Raio da sala.
            *   `Y`: (Não utilizado atualmente).
            *   `Z`: Altura da sala.
        *   **Para `Rectangular`:**
            *   `X`: Largura da sala.
            *   `Y`: Comprimento da sala.
            *   `Z`: Altura da sala.
    *   **Importante:** O valor `X` do tamanho (`Raio` ou `Largura`) também influencia o tamanho da "abertura" no túnel.

*   **`Rotation` (Rotação):**
    *   Permite que você aplique uma rotação adicional à sala. Útil para orientar salas retangulares que não estejam perfeitamente alinhadas com o túnel.

### 3. Criando Múltiplas Salas

Você pode adicionar quantas salas quiser clicando no ícone **"+"** novamente. O sistema irá gerá-las e conectá-las ao túnel com base em suas respectivas posições.

## Passo 5: Gerar e Visualizar o Resultado

1.  **Configure seus Marcadores:** Adicione uma ou duas salas ao array `Room Markers`. Dê a elas posições diferentes (ex: `0.3` e `0.8`) e tamanhos variados.
2.  **Desenhe sua Spline:** Certifique-se de que sua spline é longa o suficiente para acomodar as salas sem sobreposição.
3.  **Gere a Malha:** Se você já conectou o evento `BeginPlay` à função `Generate`, basta clicar em **Play** no editor.
4.  **Visualize:** O resultado agora deve ser um sistema de cavernas completo, com túneis de aparência orgânica que se abrem para salas maiores nas posições que você especificou. A transição entre o túnel e a sala deve ser uma malha contínua.

Se você consegue criar e configurar salas que se conectam aos túneis, **a Fase 3 foi um sucesso!**

---

**Aguardando seu feedback e aprovação para prosseguir para a Fase 4: Sistema de Assets.**
