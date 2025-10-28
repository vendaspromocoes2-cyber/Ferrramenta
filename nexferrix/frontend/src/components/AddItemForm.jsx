import React, { useState } from 'react';
import axios from 'axios';

const AddItemForm = ({ setStockData }) => {
  const [formData, setFormData] = useState({
    id: '',
    name: '',
    quantity: '',
    location: '',
  });
  const [error, setError] = useState(null);
  const [success, setSuccess] = useState(null);

  const handleChange = (e) => {
    const { name, value } = e.target;
    setFormData({ ...formData, [name]: value });
  };

  const handleSubmit = async (e) => {
    e.preventDefault();
    setError(null);
    setSuccess(null);

    try {
      const response = await axios.post('http://localhost:3000/api/v1/estoque/items', formData);

      // Atualiza o estado da tabela no App.jsx
      setStockData(prevData => [...prevData, response.data]);

      setSuccess(`Item "${response.data.name}" adicionado com sucesso!`);
      // Limpa o formulário
      setFormData({ id: '', name: '', quantity: '', location: '' });
    } catch (err) {
      setError('Erro ao adicionar o item. Verifique os dados e tente novamente.');
      console.error(err);
    }
  };

  const formStyle = {
    padding: '20px',
    margin: '20px auto',
    width: '80%',
    boxShadow: '0 2px 4px rgba(0,0,0,0.1)',
    borderRadius: '8px',
    backgroundColor: '#f9f9f9',
  };

  const inputStyle = {
    display: 'block',
    width: 'calc(100% - 20px)',
    padding: '10px',
    margin: '10px 0',
    border: '1px solid #ccc',
    borderRadius: '4px',
  };

  const buttonStyle = {
    padding: '10px 20px',
    backgroundColor: '#007bff',
    color: 'white',
    border: 'none',
    borderRadius: '4px',
    cursor: 'pointer',
  };

  return (
    <div style={formStyle}>
      <h2>Adicionar Novo Item ao Estoque</h2>
      <form onSubmit={handleSubmit}>
        <input
          type="text"
          name="id"
          placeholder="SKU do Produto"
          value={formData.id}
          onChange={handleChange}
          style={inputStyle}
          required
        />
        <input
          type="text"
          name="name"
          placeholder="Nome do Produto"
          value={formData.name}
          onChange={handleChange}
          style={inputStyle}
          required
        />
        <input
          type="number"
          name="quantity"
          placeholder="Quantidade"
          value={formData.quantity}
          onChange={handleChange}
          style={inputStyle}
        />
        <input
          type="text"
          name="location"
          placeholder="Localização"
          value={formData.location}
          onChange={handleChange}
          style={inputStyle}
        />
        <button type="submit" style={buttonStyle}>Adicionar Item</button>
      </form>
      {error && <p style={{ color: 'red', marginTop: '10px' }}>{error}</p>}
      {success && <p style={{ color: 'green', marginTop: '10px' }}>{success}</p>}
    </div>
  );
};

export default AddItemForm;
