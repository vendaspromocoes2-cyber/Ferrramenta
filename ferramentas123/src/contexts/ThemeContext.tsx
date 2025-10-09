import React, { createContext, useContext, useState, useEffect, ReactNode } from 'react';

interface ThemeColors {
  primary: string;
  background: string;
  surface: string;
  text: string;
  textSecondary: string;
  border: string;
  hover: string;
  success: string;
  warning: string;
  danger: string;
  cardBg: string;
  inputBg: string;
}

interface Theme {
  name: string;
  colors: ThemeColors;
}

interface ThemeContextType {
  theme: Theme;
  currentTheme: string;
  toggleTheme: () => void;
}

const themes: Record<string, Theme> = {
  colorido: {
    name: 'colorido',
    colors: {
      primary: '#0080FF',
      background: '#FFFFFF',
      surface: '#F5F5F5',
      text: '#1A1A1A',
      textSecondary: '#666666',
      border: '#E0E0E0',
      hover: '#0066CC',
      success: '#00C853',
      warning: '#FFB300',
      danger: '#FF3D00',
      cardBg: '#FFFFFF',
      inputBg: '#FFFFFF',
    }
  },
  black: {
    name: 'black',
    colors: {
      primary: '#0080FF',
      background: '#0A0A0A',
      surface: '#1A1A1A',
      text: '#FFFFFF',
      textSecondary: '#B0B0B0',
      border: '#333333',
      hover: '#0099FF',
      success: '#00E676',
      warning: '#FFD740',
      danger: '#FF5252',
      cardBg: '#1F1F1F',
      inputBg: '#2A2A2A',
    }
  }
};

const ThemeContext = createContext<ThemeContextType | null>(null);

export const ThemeProvider: React.FC<{ children: ReactNode }> = ({ children }) => {
  const [currentTheme, setCurrentTheme] = useState<string>('colorido');

  useEffect(() => {
    const savedTheme = localStorage.getItem('ferramentas123-theme');
    if (savedTheme && themes[savedTheme]) {
      setCurrentTheme(savedTheme);
    }
  }, []);

  const toggleTheme = () => {
    const newTheme = currentTheme === 'colorido' ? 'black' : 'colorido';
    setCurrentTheme(newTheme);
    localStorage.setItem('ferramentas123-theme', newTheme);
  };

  const theme = themes[currentTheme];

  return (
    <ThemeContext.Provider value={{ theme, currentTheme, toggleTheme }}>
      {children}
    </ThemeContext.Provider>
  );
};

export const useTheme = (): ThemeContextType => {
  const context = useContext(ThemeContext);
  if (!context) {
    throw new Error('useTheme deve ser usado dentro de ThemeProvider');
  }
  return context;
};