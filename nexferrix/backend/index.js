const express = require('express');
const app = express();
const port = 3000;

app.get('/api/v1/health-check', (req, res) => {
  res.json({ status: 'ok' });
});

app.listen(port, () => {
  console.log(`Server listening at http://localhost:${port}`);
});
