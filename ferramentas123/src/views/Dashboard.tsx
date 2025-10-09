import React from 'react';
import { useTheme } from '../contexts/ThemeContext';
import { useAuth } from '../contexts/AuthContext';
import { Button } from '../components/Button';
import { Card } from '../components/Card';

export const Dashboard: React.FC = () => {
  const { theme, toggleTheme, isBlackMode } = useTheme();
  const { user, logout } = useAuth();

  const containerStyle: React.CSSProperties = {
    minHeight: '100vh',
    backgroundColor: theme.colors.background,
    padding: '40px',
    transition: 'all 0.3s ease',
  };

  const headerStyle: React.CSSProperties = {
    display: 'flex',
    justifyContent: 'space-between',
    alignItems: 'center',
    marginBottom: '40px',
    maxWidth: '1200px',
    margin: '0 auto 40px',
  };

  const titleStyle: React.CSSProperties = {
    fontSize: '32px',
    fontWeight: 'bold',
    color: theme.colors.text,
  };

  const userInfoStyle: React.CSSProperties = {
    display: 'flex',
    alignItems: 'center',
    gap: '16px',
  };

  const gridStyle: React.CSSProperties = {
    display: 'grid',
    gridTemplateColumns: 'repeat(auto-fit, minmax(280px, 1fr))',
    gap: '24px',
    maxWidth: '1200px',
    margin: '0 auto',
  };

  const modules = [
    { id: 1, name: '🛠️ Ferramentas', description: 'Gerenciar ferramentas' },
    { id: 2, name: '📋 Solicitações', description: 'Pedidos e requisições' },
    { id: 3, name: '📦 Estoque', description: 'Controle de inventário' },
    { id: 4, name: '💳 Pagamentos', description: 'Gestão financeira' },
    { id: 5, name: '⚙️ Configurações', description: 'Ajustes do sistema' },
    { id: 6, name: '📊 Relatórios', description: 'Análises e dashboards' },
  ];

  return (
    <div style={containerStyle}>
      <div style={headerStyle}>
        <div>
          <h1 style={titleStyle}>Painel Principal</h1>
          <p style={{ color: theme.colors.textSecondary, marginTop: '8px' }}>
            Bem-vindo, <strong>{user?.username}</strong>!
          </p>
        </div>
        
        <div style={userInfoStyle}>
          <Button onClick={toggleTheme} variant="secondary">
            {isBlackMode ? '☀️ Colorido' : '🌙 Black'}
          </Button>
          <Button onClick={logout} variant="danger">
            Sair
          </Button>
        </div>
      </div>

      <div style={gridStyle}>
        {modules.map((module) => (
          <Card
            key={module.id}
            title={module.name}
            onClick={() => alert(`Módulo: ${module.name}\n(Em desenvolvimento)`)}
          >
            <p style={{ color: theme.colors.textSecondary }}>
              {module.description}
            </p>
          </Card>
        ))}
      </div>
    </div>
  );
};