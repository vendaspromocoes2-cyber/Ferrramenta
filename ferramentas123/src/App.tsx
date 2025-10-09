import React from 'react';
import { ThemeProvider, useTheme } from './contexts/ThemeContext';
import { Button } from './components/Button';
import { Card } from './components/Card';

const AppContent: React.FC = () => {
  const { theme, toggleTheme, isBlackMode } = useTheme();

  const appStyle: React.CSSProperties = {
    minHeight: '100vh',
    backgroundColor: theme.colors.background,
    color: theme.colors.text,
    padding: '40px',
    transition: 'all 0.3s ease',
  };

  return (
    <div style={appStyle}>
      <div style={{ maxWidth: '1200px', margin: '0 auto' }}>
        <div style={{ 
          display: 'flex', 
          justifyContent: 'space-between', 
          alignItems: 'center',
          marginBottom: '40px'
        }}>
          <h1 style={{ fontSize: '32px', fontWeight: 'bold' }}>
            Ferramentas123
          </h1>
          <Button onClick={toggleTheme}>
            {isBlackMode ? '☀️ Modo Colorido' : '🌙 Modo Black'}
          </Button>
        </div>

        <div style={{ 
          display: 'grid', 
          gridTemplateColumns: 'repeat(auto-fit, minmax(300px, 1fr))',
          gap: '20px'
        }}>
          <Card title="Bem-vindo!">
            <p style={{ color: theme.colors.textSecondary }}>
              Sistema de gerenciamento de ferramentas
            </p>
          </Card>

          <Card title="Teste o tema">
            <p style={{ color: theme.colors.textSecondary }}>
              Clique no botão acima para alternar entre os temas
            </p>
          </Card>
        </div>
      </div>
    </div>
  );
};

function App() {
  return (
    <ThemeProvider>
      <AppContent />
    </ThemeProvider>
  );
}

export default App;