const express = require('express');
const cors = require('cors');
const app = express();
const port = 3000;

app.use(cors());

const mockStockData = [
  { id: 'SKU-001', name: 'Parafuso Sextavado 1/4"', quantity: 1500, location: 'A1-01' },
  { id: 'SKU-002', name: 'Porca 1/4"', quantity: 2500, location: 'A1-02' },
  { id: 'SKU-003', name: 'Arruela Lisa 1/4"', quantity: 3000, location: 'A1-03' },
  { id: 'SKU-004', name: 'Chave de Fenda Phillips', quantity: 150, location: 'B2-05' },
  { id: 'SKU-005', name: 'Trena 5m', quantity: 80, location: 'B2-08' },
  { id: 'SKU-006', name: 'Martelo Unha', quantity: 120, location: 'B2-09' },
  { id: 'SKU-007', name: 'Óleo Lubrificante WD-40', quantity: 200, location: 'C3-15' },
];

app.get('/api/v1/health-check', (req, res) => {
  res.json({ status: 'ok' });
});

app.get('/api/v1/relatorios/estoque', (req, res) => {
  res.json(mockStockData);
});

app.listen(port, () => {
  console.log(`Server listening at http://localhost:${port}`);
});
