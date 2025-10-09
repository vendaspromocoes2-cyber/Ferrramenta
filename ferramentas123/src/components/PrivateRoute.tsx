import React from 'react';
import { useAuth } from '../contexts/AuthContext';
import { Login } from '../views/Login';

interface PrivateRouteProps {
  children: React.ReactNode;
}

export const PrivateRoute: React.FC<PrivateRouteProps> = ({ children }) => {
  const { isAuthenticated, isLoading } = useAuth();

  if (isLoading) {
    return (
      <div style={{
        minHeight: '100vh',
        display: 'flex',
        alignItems: 'center',
        justifyContent: 'center',
        fontSize: '18px',
      }}>
        Carregando...
      </div>
    );
  }

  return isAuthenticated ? <>{children}</> : <Login />;
};