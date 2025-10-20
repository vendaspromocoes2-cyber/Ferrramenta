import React, { useState } from 'react';
import { useTheme } from '../contexts/ThemeContext';
import { useAuth } from '../contexts/AuthContext';
import { Button } from '../components/Button';
import { Input } from '../components/Input';
import { Card } from '../components/Card';

export const Login: React.FC = () => {
  const { theme, toggleTheme, isBlackMode } = useTheme();
  const { login } = useAuth();
  const [username, setUsername] = useState('');
  const [password, setPassword] = useState('');
  const [error, setError] = useState('');
  const [isLoading, setIsLoading] = useState(false);

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    setError('');
    setIsLoading(true);

    const success = await login(username, password);
    
    if (!success) {
      setError('Usuário ou senha inválidos');
    }
    
    setIsLoading(false);
  };

  const containerStyle: React.CSSProperties = {
    minHeight: '100vh',
    backgroundColor: theme.colors.background,
    display: 'flex',
    alignItems: 'center',
    justifyContent: 'center',
    padding: '20px',
  };

  const loginBoxStyle: React.CSSProperties = {
    width: '100%',
    maxWidth: '400px',
  };

  const headerStyle: React.CSSProperties = {
    textAlign: 'center',
    marginBottom: '32px',
  };

  const titleStyle: React.CSSProperties = {
    fontSize: '32px',
    fontWeight: 'bold',
    color: theme.colors.text,
    marginBottom: '8px',
  };

  const subtitleStyle: React.CSSProperties = {
    fontSize: '16px',
    color: theme.colors.textSecondary,
  };

  const themeButtonStyle: React.CSSProperties = {
    position: 'absolute',
    top: '20px',
    right: '20px',
  };

  return (
    <div style={containerStyle}>
      <div style={themeButtonStyle}>
        <Button onClick={toggleTheme} variant="secondary">
          {isBlackMode ? '☀️ Colorido' : '🌙 Black'}
        </Button>
      </div>
      
      <div style={loginBoxStyle}>
        <div style={headerStyle}>
          <h1 style={titleStyle}>Ferramentas123</h1>
          <p style={subtitleStyle}>Sistema de Gerenciamento</p>
        </div>

        <Card title="Login">
          <form onSubmit={handleSubmit}>
            <Input
              label="Usuário"
              value={username}
              onChange={setUsername}
              placeholder="Digite seu usuário"
              disabled={isLoading}
            />
            
            <Input
              label="Senha"
              type="password"
              value={password}
              onChange={setPassword}
              placeholder="Digite sua senha"
              disabled={isLoading}
              error={error}
            />

            <Button 
              type="submit" 
              fullWidth 
              disabled={isLoading || !username || !password}
            >
              {isLoading ? 'Entrando...' : 'Entrar'}
            </Button>

            {error && (
              <div style={{ 
                marginTop: '16px', 
                padding: '12px', 
                backgroundColor: theme.colors.danger + '20',
                borderRadius: '8px',
                color: theme.colors.danger,
                textAlign: 'center',
              }}>
                {error}
              </div>
            )}

            <div style={{
              marginTop: '16px',
              textAlign: 'center',
              fontSize: '14px',
              color: theme.colors.textSecondary,
            }}>
              Usuário padrão: <strong>admin</strong><br />
              Senha padrão: <strong>123456</strong>
            </div>
          </form>
        </Card>
      </div>
    </div>
  );
};