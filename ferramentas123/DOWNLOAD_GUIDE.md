# Download de Arquivos - Guia de Uso

## Descrição

Este módulo fornece funcionalidades completas para download de arquivos no sistema Ferramentas123. É possível baixar arquivos de diferentes formas:

- Arquivos de texto (TXT)
- Arquivos JSON
- Arquivos CSV
- Arquivos de URLs externas
- Arquivos a partir de Blobs
- Arquivos de respostas de API

## Como Usar

### Importando as Funções

```typescript
import {
  downloadTextFile,
  downloadJsonFile,
  downloadCsvFile,
  downloadFileFromUrl,
  downloadFileFromBlob,
  downloadFileFromResponse
} from '../utils/fileDownload';
```

### Exemplos de Uso

#### 1. Download de Arquivo de Texto

```typescript
const content = 'Conteúdo do arquivo de texto';
downloadTextFile(content, 'meu-arquivo.txt');

// Com tipo MIME customizado
downloadTextFile(content, 'arquivo.html', 'text/html');
```

#### 2. Download de Arquivo JSON

```typescript
const data = {
  nome: 'Exemplo',
  valores: [1, 2, 3]
};
downloadJsonFile(data, 'dados.json');
```

#### 3. Download de Arquivo CSV

```typescript
const dados = [
  ['Nome', 'Idade', 'Cidade'],
  ['João', '30', 'São Paulo'],
  ['Maria', '25', 'Rio de Janeiro']
];
downloadCsvFile(dados, 'tabela.csv');
```

#### 4. Download de URL

```typescript
downloadFileFromUrl('https://exemplo.com/arquivo.pdf', 'documento.pdf');
```

#### 5. Download de Blob

```typescript
const blob = new Blob(['conteúdo'], { type: 'text/plain' });
downloadFileFromBlob(blob, 'arquivo.txt');
```

#### 6. Download de Resposta de API

```typescript
const response = await fetch('https://api.exemplo.com/relatorio');
await downloadFileFromResponse(response, 'relatorio.pdf');
```

## Componente de Demonstração

O componente `DownloadDemo` fornece uma interface completa para testar todas as funcionalidades de download.

### Uso no Dashboard

```typescript
import { DownloadDemo } from '../components/DownloadDemo';

// No componente
const [showDownloads, setShowDownloads] = useState(false);

// No JSX
{showDownloads && (
  <DownloadDemo onClose={() => setShowDownloads(false)} />
)}
```

## Funcionalidades do Módulo Downloads

1. **Download de Arquivos de Exemplo**: Gera e baixa arquivos TXT, JSON e CSV com dados de exemplo
2. **Download de URL**: Permite baixar arquivos de URLs externas
3. **Interface Intuitiva**: Interface amigável com instruções claras
4. **Feedback Visual**: Botões com ícones e descrições claras

## Testes

Testes unitários estão disponíveis em `src/utils/fileDownload.test.ts` e cobrem:

- Download de arquivos de texto
- Download de arquivos JSON
- Download de arquivos CSV
- Download a partir de Blobs
- Uso de tipos MIME customizados

Execute os testes com:

```bash
npm test -- fileDownload.test.ts
```

## Segurança

- Todas as funções são executadas no lado do cliente
- Não há upload de dados para servidores externos
- URLs externas devem ser validadas antes do uso
- O CodeQL foi executado e nenhuma vulnerabilidade foi encontrada

## Compatibilidade

- React 19+
- TypeScript 4.9+
- Navegadores modernos (Chrome, Firefox, Safari, Edge)
- Suporte para download de arquivos em dispositivos móveis

## Notas Técnicas

- Os downloads usam a API de Blobs do navegador
- Links temporários são criados e revogados automaticamente
- Arquivos são gerados dinamicamente no lado do cliente
- Não há limite de tamanho para downloads (limitado apenas pela memória do navegador)
