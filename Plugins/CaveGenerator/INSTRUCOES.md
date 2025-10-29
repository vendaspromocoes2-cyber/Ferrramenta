# Como Integrar e Usar o Plugin CaveGenerator (Fase 5 - Refinamentos)

Esta versão final introduz melhorias de qualidade de vida, visuais e de performance.

---
## Refinamento 1: Geração no Editor (Botão "Generate")

Como usar o novo fluxo de trabalho:

1.  Selecione seu `BP_CaveTunnel` na cena.
2.  No painel **Details**, na categoria **"Cave Generation|Actions"**, você verá um novo botão chamado **`Generate`**. Clique nele a qualquer momento para reconstruir a caverna com as configurações atuais.
3.  **Ainda melhor:** A caverna agora se **regenera automaticamente** sempre que você altera qualquer propriedade no painel Details (como a `RuggednessAmount`, a escala de um asset, ou a posição de uma sala). Isso permite que você itere e projete sua caverna em tempo real, sem precisar iniciar o modo "Play".

---
## Refinamento 2: Mapeamento de Textura Triplanar (UVs Melhorados)

Para eliminar o estiramento de texturas, o plugin agora usa uma técnica de mapeamento triplanar. Isso requer um material especial. Siga estes passos para criá-lo:

### 1. Crie um Novo Material

1.  No **Content Browser**, clique com o botão direito e crie um novo **Material**. Chame-o de `M_CaveMaterial_Triplanar`.
2.  Abra o material.

### 2. Construa a Lógica Triplanar

Você precisará construir a seguinte rede de nós. A ideia é projetar a mesma textura de três direções (X, Y, Z) e misturá-las com base na direção da superfície.

1.  **Nó `WorldPosition`:** Adicione este nó. Ele nos dá a posição de cada pixel no mundo.
2.  **Nó `VertexNormalWS`:** Adicione este nó. Ele nos dá a direção (normal) da superfície.
3.  **Nós de Textura:**
    *   Crie um nó `TextureSample` para sua textura de rocha (Albedo/Cor). Para torná-lo um parâmetro, clique com o botão direito nele e selecione **"Convert to Parameter"**. Dê um nome, como `Rock_Albedo`.
    *   Faça o mesmo para a textura de Normal (`Rock_Normal`) e, se tiver, para a de Roughness (`Rock_Roughness`).
4.  **Lógica de Projeção:**
    *   **Projeção XY (Chão/Teto):**
        *   Pegue o `WorldPosition`, use um nó `ComponentMask` para pegar apenas os canais **R e G (X e Y)**. Conecte isso ao pino `UVs` da sua `TextureSample`.
    *   **Projeção XZ (Paredes Laterais):**
        *   Pegue o `WorldPosition`, use um `ComponentMask` para pegar os canais **R e B (X e Z)**. Conecte isso ao `UVs` de uma *segunda* `TextureSample` (usando a mesma textura).
    *   **Projeção YZ (Paredes Frontais/Traseiras):**
        *   Pegue o `WorldPosition`, use um `ComponentMask` para pegar os canais **G e B (Y e Z)**. Conecte isso ao `UVs` de uma *terceira* `TextureSample`.
5.  **Lógica de Mistura (Blend):**
    *   Pegue o `VertexNormalWS`. Use um nó `Abs` (Absolute) para garantir que todos os valores sejam positivos.
    *   Use um nó `Power` com o resultado do `Abs`. Um expoente alto (ex: `5` a `10`) cria uma transição mais nítida. Isso será nosso "Blend Mask".
    *   Use o resultado do `Power` como `Alpha` em nós `LinearInterpolate` (Lerp) para misturar as três projeções de textura. Você precisará de dois Lerps:
        *   O primeiro Lerp mistura a projeção XZ e YZ usando o canal **R** da máscara de blend.
        *   O segundo Lerp mistura o resultado do primeiro Lerp com a projeção XY usando o canal **B** da máscara de blend.
6.  **Conecte o Resultado:** Conecte o resultado final do Lerp ao pino `Base Color` do seu material. Repita a mesma lógica de Lerp para os mapas de Normal e Roughness.

### 3. Use o Novo Material

1.  Salve seu material `M_CaveMaterial_Triplanar`.
2.  No seu `BP_CaveTunnel`, selecione o `Cave Generator Component`.
3.  Na propriedade **`Tunnel Material`**, selecione o seu novo material triplanar.
4.  Clique no botão **`Generate`**. A textura da sua caverna agora deve parecer uniforme, sem estiramentos e com uma escala consistente em todas as superfícies.

---
**Aguardando seu feedback final para a entrega do plugin completo.**
