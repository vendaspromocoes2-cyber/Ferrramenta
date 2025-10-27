import React from 'react';
import { ThemeProvider } from './contexts/ThemeContext';
import { AuthProvider } from './contexts/AuthContext';
import { PrivateRoute } from './components/PrivateRoute';
import { Dashboard } from './views/Dashboard';

function App() {
  return (
    <ThemeProvider>
      <AuthProvider>
        <PrivateRoute>
          <Dashboard />
        </PrivateRoute>
      </AuthProvider>
    </ThemeProvider>
  );
}

export default App;