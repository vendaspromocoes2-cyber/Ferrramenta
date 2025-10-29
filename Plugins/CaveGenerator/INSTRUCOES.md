# Como Integrar e Usar o Plugin CaveGenerator (Fase 4 - Sistema de Assets)

Esta versão introduz o sistema de distribuição procedural de assets, que irá popular sua caverna com objetos como rochas, cristais e outros detalhes.

## Instalação

Se você já instalou as fases anteriores, apenas substitua os arquivos na pasta `Plugins/CaveGenerator` pelos novos e **recompile o projeto** no Visual Studio/Rider. Se for a primeira vez, siga os passos da Fase 1.

---

## Passo 4: Usando o Sistema de Distribuição de Assets

Abra o seu Blueprint `BP_CaveTunnel` e selecione o `Cave Generator Component`. No painel **Details**, abaixo das categorias existentes, você encontrará a nova categoria **`Assets`**.

### 1. Preparando sua Pasta de Conteúdo

1.  No seu **Content Browser**, crie uma nova pasta. Dê um nome claro, como `Props/CaveRocks`.
2.  **Importe ou mova** todas as `Static Meshes` (malhas estáticas) que você quer que apareçam na sua caverna para dentro desta pasta. O sistema irá escanear esta pasta e usar todos os modelos que encontrar.

### 2. Configurando os Parâmetros no Editor

Na categoria `Assets` do componente, você encontrará as seguintes propriedades:

*   **`Asset Folder` (Pasta de Assets):**
    *   Este é o passo mais importante. Clique no ícone de pasta ou no menu dropdown e **selecione a pasta** que você criou no passo anterior (`Props/CaveRocks`).

*   **`Asset Density` (Densidade de Assets):**
    *   Controla **quantos** objetos serão colocados. O valor é uma aproximação de "objetos por metro quadrado".
    *   `0.01` é um bom ponto de partida para objetos maiores como rochas.
    *   `0.1` ou mais pode ser usado para detalhes menores como seixos.
    *   **Comece com valores baixos** e aumente gradualmente para evitar criar milhares de objetos de uma vez.

*   **`Scale Range` (Faixa de Escala):**
    *   Controla a variação de tamanho dos objetos para que não pareçam todos iguais.
    *   `X` é a escala mínima (ex: `0.8` para 80% do tamanho original).
    *   `Y` é a escala máxima (ex: `1.5` para 150% do tamanho original).
    *   Cada objeto terá uma escala uniforme aleatória escolhida entre esses dois valores.

*   **`Z Offset` (Deslocamento Z):**
    *   Permite ajustar a altura de cada objeto.
    *   Um valor negativo (ex: `-10.0`) é muito útil para **enterrar um pouco a base** das rochas no chão, criando uma aparência mais natural.

*   **`Random Seed` (Semente Aleatória):**
    *   O número usado para inicializar o gerador de aleatoriedade.
    *   Mudar este número irá gerar uma **distribuição completamente diferente** usando os mesmos parâmetros. Se você encontrar uma distribuição que goste, mantenha a mesma semente para que o resultado seja sempre o mesmo.

## Passo 5: Gerar e Visualizar o Resultado

1.  **Configure os Parâmetros:** Selecione sua pasta de assets, ajuste a densidade, escala e outros valores como desejar.
2.  **Gere a Malha:** Clique em **Play** no editor (assumindo que o `Generate` está conectado ao `BeginPlay`).
3.  **Visualize:** Sua caverna agora deve ser gerada, e **imediatamente após, os objetos da sua pasta aparecerão espalhados pelo chão** dos túneis e salas. Eles terão tamanhos e rotações variadas, criando uma cena muito mais rica e detalhada.

Se você consegue gerar uma caverna populada com seus próprios modelos 3D, **a Fase 4 foi um sucesso!**

---

**Aguardando seu feedback e aprovação para prosseguir para a Fase 5: Refinamentos.**
