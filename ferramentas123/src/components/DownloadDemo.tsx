import React, { useState } from 'react';
import { useTheme } from '../contexts/ThemeContext';
import { Button } from './Button';
import { Input } from './Input';
import { 
  downloadTextFile, 
  downloadJsonFile, 
  downloadCsvFile,
  downloadFileFromUrl 
} from '../utils/fileDownload';

interface DownloadDemoProps {
  onClose?: () => void;
}

export const DownloadDemo: React.FC<DownloadDemoProps> = ({ onClose }) => {
  const { theme } = useTheme();
  const [url, setUrl] = useState('');
  const [filename, setFilename] = useState('');

  const handleDownloadText = () => {
    const sampleText = `Ferramentas123 - Sistema de Gerenciamento
    
Este é um arquivo de exemplo gerado em ${new Date().toLocaleString('pt-BR')}.

Funcionalidades disponíveis:
- Download de arquivos de texto
- Download de arquivos JSON
- Download de arquivos CSV
- Download de arquivos de URLs

Desenvolvido com React + TypeScript`;

    downloadTextFile(sampleText, 'exemplo.txt');
  };

  const handleDownloadJson = () => {
    const sampleData = {
      sistema: 'Ferramentas123',
      versao: '1.0.0',
      dataGeracao: new Date().toISOString(),
      modulos: [
        { id: 1, nome: 'Ferramentas', status: 'ativo' },
        { id: 2, nome: 'Solicitações', status: 'ativo' },
        { id: 3, nome: 'Estoque', status: 'ativo' },
        { id: 4, nome: 'Pagamentos', status: 'ativo' },
        { id: 5, nome: 'Configurações', status: 'ativo' },
        { id: 6, nome: 'Relatórios', status: 'ativo' },
      ],
      usuarios: [
        { id: 1, username: 'admin', role: 'admin' },
      ],
    };

    downloadJsonFile(sampleData, 'dados-sistema.json');
  };

  const handleDownloadCsv = () => {
    const csvData = [
      ['ID', 'Módulo', 'Descrição', 'Status'],
      ['1', 'Ferramentas', 'Gerenciar ferramentas', 'Ativo'],
      ['2', 'Solicitações', 'Pedidos e requisições', 'Ativo'],
      ['3', 'Estoque', 'Controle de inventário', 'Ativo'],
      ['4', 'Pagamentos', 'Gestão financeira', 'Ativo'],
      ['5', 'Configurações', 'Ajustes do sistema', 'Ativo'],
      ['6', 'Relatórios', 'Análises e dashboards', 'Ativo'],
    ];

    downloadCsvFile(csvData, 'modulos.csv');
  };

  const handleDownloadFromUrl = () => {
    if (!url) {
      alert('Por favor, insira uma URL válida');
      return;
    }

    try {
      downloadFileFromUrl(url, filename || undefined);
    } catch (error) {
      alert('Erro ao baixar arquivo. Verifique se a URL está correta.');
    }
  };

  const containerStyle: React.CSSProperties = {
    backgroundColor: theme.colors.cardBg,
    borderRadius: '12px',
    padding: '24px',
    border: `1px solid ${theme.colors.border}`,
  };

  const sectionStyle: React.CSSProperties = {
    marginBottom: '24px',
  };

  const titleStyle: React.CSSProperties = {
    fontSize: '18px',
    fontWeight: '600',
    color: theme.colors.text,
    marginBottom: '12px',
  };

  const descriptionStyle: React.CSSProperties = {
    fontSize: '14px',
    color: theme.colors.textSecondary,
    marginBottom: '16px',
  };

  const buttonGroupStyle: React.CSSProperties = {
    display: 'flex',
    gap: '12px',
    flexWrap: 'wrap',
  };

  return (
    <div style={containerStyle}>
      <div style={{ 
        display: 'flex', 
        justifyContent: 'space-between', 
        alignItems: 'center',
        marginBottom: '24px'
      }}>
        <h2 style={{ fontSize: '24px', fontWeight: 'bold', color: theme.colors.text }}>
          📥 Download de Arquivos
        </h2>
        {onClose && (
          <Button onClick={onClose} variant="secondary">
            Fechar
          </Button>
        )}
      </div>

      <div style={sectionStyle}>
        <h3 style={titleStyle}>Download de Arquivos de Exemplo</h3>
        <p style={descriptionStyle}>
          Baixe arquivos de exemplo gerados dinamicamente
        </p>
        <div style={buttonGroupStyle}>
          <Button onClick={handleDownloadText}>
            📄 Baixar TXT
          </Button>
          <Button onClick={handleDownloadJson}>
            📋 Baixar JSON
          </Button>
          <Button onClick={handleDownloadCsv}>
            📊 Baixar CSV
          </Button>
        </div>
      </div>

      <div style={sectionStyle}>
        <h3 style={titleStyle}>Download de URL</h3>
        <p style={descriptionStyle}>
          Insira uma URL para baixar um arquivo externo
        </p>
        <Input
          label="URL do arquivo"
          value={url}
          onChange={setUrl}
          placeholder="https://exemplo.com/arquivo.pdf"
        />
        <Input
          label="Nome do arquivo (opcional)"
          value={filename}
          onChange={setFilename}
          placeholder="meu-arquivo.pdf"
        />
        <Button onClick={handleDownloadFromUrl} fullWidth>
          ⬇️ Baixar de URL
        </Button>
      </div>

      <div style={{
        marginTop: '24px',
        padding: '16px',
        backgroundColor: theme.colors.surface,
        borderRadius: '8px',
        fontSize: '14px',
        color: theme.colors.textSecondary,
      }}>
        <strong style={{ color: theme.colors.text }}>💡 Dica:</strong> 
        {' '}Todas as funções de download estão disponíveis em{' '}
        <code style={{ 
          backgroundColor: theme.colors.background,
          padding: '2px 6px',
          borderRadius: '4px',
          fontFamily: 'monospace'
        }}>
          src/utils/fileDownload.ts
        </code>
        {' '}e podem ser importadas em qualquer componente.
      </div>
    </div>
  );
};
