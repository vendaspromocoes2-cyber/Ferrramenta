import React, { useState, useEffect } from 'react';
import axios from 'axios';

const StockReport = ({ stockData, setStockData }) => {
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState(null);

  useEffect(() => {
    const fetchData = async () => {
      try {
        const response = await axios.get('http://localhost:3000/api/v1/relatorios/estoque');
        setStockData(response.data);
      } catch (err) {
        setError('Erro ao buscar dados do estoque.');
        console.error(err);
      } finally {
        setLoading(false);
      }
    };

    if (stockData.length === 0) {
      fetchData();
    } else {
      setLoading(false);
    }
  }, [stockData, setStockData]);

  if (loading) {
    return <p>Carregando relatório de estoque...</p>;
  }

  if (error) {
    return <p style={{ color: 'red' }}>{error}</p>;
  }

  const tableStyle = {
    width: '80%',
    margin: '20px auto',
    borderCollapse: 'collapse',
    boxShadow: '0 2px 4px rgba(0,0,0,0.1)',
  };

  const thStyle = {
    border: '1px solid #ddd',
    padding: '12px',
    backgroundColor: '#f7f7f7',
    textAlign: 'left',
    fontWeight: 'bold',
  };

  const tdStyle = {
    border: '1px solid #ddd',
    padding: '12px',
  };

  return (
    <div style={{ padding: '20px' }}>
      <h1>Relatório de Estoque - NexFerriX</h1>
      <table style={tableStyle}>
        <thead>
          <tr>
            <th style={thStyle}>SKU</th>
            <th style={thStyle}>Nome do Produto</th>
            <th style={thStyle}>Quantidade</th>
            <th style={thStyle}>Localização</th>
          </tr>
        </thead>
        <tbody>
          {stockData.map((item) => (
            <tr key={item.id}>
              <td style={tdStyle}>{item.id}</td>
              <td style={tdStyle}>{item.name}</td>
              <td style={tdStyle}>{item.quantity}</td>
              <td style={tdStyle}>{item.location}</td>
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
};

export default StockReport;
