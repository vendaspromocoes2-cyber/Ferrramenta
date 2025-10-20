import React from 'react';
import { useTheme } from '../contexts/ThemeContext';

interface ButtonProps {
  children: React.ReactNode;
  onClick?: () => void;
  variant?: 'primary' | 'secondary' | 'danger';
  type?: 'button' | 'submit' | 'reset';
  disabled?: boolean;
  fullWidth?: boolean;
}

export const Button: React.FC<ButtonProps> = ({
  children,
  onClick,
  variant = 'primary',
  type = 'button',
  disabled = false,
  fullWidth = false,
}) => {
  const { theme } = useTheme();

  const getButtonColor = () => {
    switch (variant) {
      case 'primary':
        return theme.colors.primary;
      case 'danger':
        return theme.colors.danger;
      case 'secondary':
        return theme.colors.surface;
      default:
        return theme.colors.primary;
    }
  };

  const getTextColor = () => {
    return variant === 'secondary' ? theme.colors.text : '#FFFFFF';
  };

  const buttonStyle: React.CSSProperties = {
    backgroundColor: getButtonColor(),
    color: getTextColor(),
    border: variant === 'secondary' ? `1px solid ${theme.colors.border}` : 'none',
    padding: '12px 24px',
    borderRadius: '8px',
    fontSize: '16px',
    fontWeight: '600',
    cursor: disabled ? 'not-allowed' : 'pointer',
    opacity: disabled ? 0.5 : 1,
    width: fullWidth ? '100%' : 'auto',
    transition: 'all 0.3s ease',
  };

  return (
    <button
      type={type}
      onClick={onClick}
      disabled={disabled}
      style={buttonStyle}
      onMouseEnter={(e) => {
        if (!disabled) {
          e.currentTarget.style.opacity = '0.9';
          e.currentTarget.style.transform = 'translateY(-2px)';
        }
      }}
      onMouseLeave={(e) => {
        e.currentTarget.style.opacity = '1';
        e.currentTarget.style.transform = 'translateY(0)';
      }}
    >
      {children}
    </button>
  );
};