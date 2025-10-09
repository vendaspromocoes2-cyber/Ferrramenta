export interface Theme {
  name: string;
  colors: {
    primary: string;
    background: string;
    surface: string;
    text: string;
    textSecondary: string;
    border: string;
    error: string;
    success: string;
    warning: string;
  };
}

export const coloridoTheme: Theme = {
  name: 'colorido',
  colors: {
    primary: '#0080FF',
    background: '#F5F5F5',
    surface: '#FFFFFF',
    text: '#333333',
    textSecondary: '#666666',
    border: '#E0E0E0',
    error: '#FF4444',
    success: '#00C851',
    warning: '#FFBB33',
  },
};

export const blackTheme: Theme = {
  name: 'black',
  colors: {
    primary: '#0080FF',
    background: '#0A0A0A',
    surface: '#1A1A1A',
    text: '#FFFFFF',
    textSecondary: '#B0B0B0',
    border: '#333333',
    error: '#FF4444',
    success: '#00C851',
    warning: '#FFBB33',
  },
};