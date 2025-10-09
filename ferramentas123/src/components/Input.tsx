import React from 'react';
import { useTheme } from '../contexts/ThemeContext';

interface InputProps {
  type?: string;
  placeholder?: string;
  value: string;
  onChange: (value: string) => void;
  label?: string;
  error?: string;
  disabled?: boolean;
}

export const Input: React.FC<InputProps> = ({
  type = 'text',
  placeholder,
  value,
  onChange,
  label,
  error,
  disabled = false,
}) => {
  const { theme } = useTheme();

  const inputStyle: React.CSSProperties = {
    width: '100%',
    padding: '12px 16px',
    fontSize: '16px',
    borderRadius: '8px',
    border: `2px solid ${error ? theme.colors.error : theme.colors.border}`,
    backgroundColor: theme.colors.surface,
    color: theme.colors.text,
    outline: 'none',
    transition: 'all 0.3s ease',
  };

  const labelStyle: React.CSSProperties = {
    display: 'block',
    marginBottom: '8px',
    fontSize: '14px',
    fontWeight: '600',
    color: theme.colors.text,
  };

  const errorStyle: React.CSSProperties = {
    color: theme.colors.error,
    fontSize: '12px',
    marginTop: '4px',
  };

  return (
    <div style={{ marginBottom: '20px' }}>
      {label && <label style={labelStyle}>{label}</label>}
      <input
        type={type}
        placeholder={placeholder}
        value={value}
        onChange={(e) => onChange(e.target.value)}
        disabled={disabled}
        style={inputStyle}
        onFocus={(e) => {
          e.currentTarget.style.borderColor = theme.colors.primary;
        }}
        onBlur={(e) => {
          e.currentTarget.style.borderColor = error ? theme.colors.error : theme.colors.border;
        }}
      />
      {error && <div style={errorStyle}>{error}</div>}
    </div>
  );
};