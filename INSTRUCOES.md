# Como Integrar e Usar o Plugin CaveGenerator (Fase 2 - Túneis Realistas)

Esta versão introduz ferramentas para transformar o túnel cilíndrico básico em uma caverna com aparência orgânica e irregular.

## Passos 1, 2 e 3: Instalação (Idêntico ao anterior)

Se você já instalou a Fase 1, basta substituir os arquivos na pasta `Plugins/CaveGenerator` pelos novos e **recompilar o projeto** no Visual Studio/Rider. Se é a primeira vez, siga os passos da Fase 1 para copiar, compilar e ativar o plugin.

---

## Passo 4: Usando os Novos Recursos Realistas

Abra o seu Blueprint `BP_CaveTunnel` e selecione o `Cave Generator Component`. No painel **Details**, você encontrará as novas seções.

### 1. Controlando o Raio com uma Curva

A propriedade `Tunnel Radius` (um número único) foi substituída por **`Tunnel Radius Curve`**.

1.  **Crie um Asset de Curva:**
    *   No Content Browser, clique com o botão direito e vá em **Miscellaneous > Curve**.
    *   Na janela que aparece, selecione **`CurveFloat`** e clique em **Select**.
    *   Dê um nome ao seu novo asset, por exemplo, `C_CaveRadius`.

2.  **Edite a Curva:**
    *   Abra o asset `C_CaveRadius`. Você verá um editor de gráfico.
    *   **O eixo horizontal (Time)** representa o comprimento da sua caverna, de **0.0 (início)** a **1.0 (fim)**.
    *   **O eixo vertical (Value)** representa o **raio do túnel** naquele ponto.
    *   **Para adicionar pontos:** Clique com o botão direito no gráfico e selecione **"Add key"**.
    *   Crie uma forma interessante. Por exemplo, comece com um valor de `300`, adicione um ponto no meio (tempo `0.5`) com valor `600` para criar uma área central ampla, e termine com um valor de `250`. Isso criará um túnel que começa médio, se alarga no meio e se estreita no final.

3.  **Associe a Curva:**
    *   Volte para o seu `BP_CaveTunnel`.
    *   Na propriedade `Tunnel Radius Curve` do `Cave Generator Component`, clique no dropdown e selecione o seu asset `C_CaveRadius`.

### 2. Adicionando Rugosidade (Deformação)

Abaixo das propriedades de formato (`Shape`), você encontrará uma nova categoria: **`Ruggedness`**.

*   **`Ruggedness Amount` (Força):**
    *   Controla a **intensidade** da deformação.
    *   `0`: O túnel será perfeitamente liso (bom para debug).
    *   `50-150`: Valores bons para uma aparência de caverna rochosa.
    *   Valores muito altos podem fazer a malha se auto-intersectar. Ajuste com cuidado.

*   **`Ruggedness Scale` (Escala):**
    *   Controla o **"tamanho" ou "frequência"** das irregularidades.
    *   **Valores pequenos (ex: 50-100):** Criam uma superfície muito detalhada e barulhenta, como rocha pontiaguda.
    *   **Valores grandes (ex: 300-500):** Criam deformações mais suaves e onduladas, como colinas ou saliências graduais.

## Passo 5: Gerar e Visualizar o Resultado

1.  **Desenhe sua Spline:** Selecione o `BP_CaveTunnel` na cena e ajuste a spline como desejar.
2.  **Ajuste os Parâmetros:** Com o `Cave Generator Component` selecionado, brinque com a `Ruggedness Amount` e `Scale`.
3.  **Gere a Malha:** Se você já conectou o evento `BeginPlay` à função `Generate` (como na Fase 1), basta clicar em **Play** no editor.
4.  **Visualize:** Seu túnel agora deve aparecer com largura variável (controlada pela sua curva) e uma superfície deformada e rochosa. A iluminação deve interagir corretamente com as novas irregularidades, criando sombras e realces que dão profundidade à cena.

Se você consegue gerar um túnel com raio variável e uma superfície com aparência orgânica e rochosa, **a Fase 2 foi um sucesso!**

---

**Aguardando seu feedback e aprovação para prosseguir para a Fase 3: Sistema de Salas.**
