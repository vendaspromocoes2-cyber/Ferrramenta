import React, { useState } from 'react';
import StockReport from './components/StockReport';
import AddItemForm from './components/AddItemForm';
import './App.css';

function App() {
  const [stockData, setStockData] = useState([]);

  return (
    <div className="App">
      <AddItemForm setStockData={setStockData} />
      <StockReport stockData={stockData} setStockData={setStockData} />
    </div>
  );
}

export default App;
