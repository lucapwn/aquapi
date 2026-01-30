# AquaPi Dashboard

Sistema de gerenciamento de irrigação automatizada com interface moderna e intuitiva.

## 🚀 Tecnologias

- **Next.js 14** - Framework React
- **TypeScript** - Tipagem estática
- **Shadcn UI** - Componentes de interface
- **Tailwind CSS** - Estilização
- **Recharts** - Gráficos
- **date-fns** - Manipulação de datas

## 📋 Pré-requisitos

- Node.js 18+ instalado
- npm ou yarn
- API backend rodando em `http://localhost:3000/api`

## 🔧 Instalação

### 1. Clone ou extraia o projeto

```bash
cd aquapi-dashboard
```

### 2. Instale as dependências

```bash
npm install
```

ou

```bash
yarn install
```

### 3. Configure a API

Certifique-se de que sua API está rodando em `http://localhost:3000/api` com os seguintes endpoints:

- **GET** `/api/sensors` - Retorna dados dos sensores
- **POST** `/api/watering` - Atualiza configuração de irrigação

Se sua API estiver em outra porta ou domínio, edite a constante `API_BASE_URL` no arquivo `app/page.tsx`.

## 🎯 Executar o Projeto

### Modo de desenvolvimento

```bash
npm run dev
```

ou

```bash
yarn dev
```

Acesse [http://localhost:3001](http://localhost:3001) no navegador.

> **Nota:** O projeto roda na porta 3001 para não conflitar com a API na porta 3000.

### Build para produção

```bash
npm run build
npm run start
```

## 📱 Funcionalidades

### Dashboard
- Visualização em tempo real de:
  - Temperatura
  - Umidade do ar
  - Luminosidade
  - Umidade do solo
- Status da conexão do dispositivo
- Estado da bomba de irrigação
- Gráfico histórico dos sensores
- Atualização automática a cada 5 segundos

### Irrigação
- Configuração de valores de ativação/desativação
- Controle manual do sistema (ativar/desativar)
- Validação de erros
- Visualização do status atual
- Previsão da próxima ação automática

## 🏗️ Estrutura do Projeto

```
aquapi-dashboard/
├── app/
│   ├── globals.css          # Estilos globais
│   ├── layout.tsx           # Layout raiz
│   └── page.tsx             # Página principal
├── components/
│   ├── ui/                  # Componentes Shadcn UI
│   │   ├── alert.tsx
│   │   ├── badge.tsx
│   │   ├── button.tsx
│   │   ├── card.tsx
│   │   ├── dialog.tsx
│   │   ├── label.tsx
│   │   ├── slider.tsx
│   │   ├── switch.tsx
│   │   └── tabs.tsx
│   ├── dashboard-tab.tsx    # Tab Dashboard
│   ├── irrigation-tab.tsx   # Tab Irrigação
│   ├── metric-card.tsx      # Card de métricas
│   ├── sensor-chart.tsx     # Gráfico de sensores
│   ├── system-status.tsx    # Status do sistema
│   └── watering-control.tsx # Controle de irrigação
├── lib/
│   └── utils.ts             # Funções utilitárias
├── types/
│   └── index.ts             # Tipos TypeScript
├── package.json
├── tailwind.config.ts
└── tsconfig.json
```

## 🔌 API

### GET /api/sensors

Retorna os dados atuais dos sensores.

**Resposta:**
```json
{
  "data": {
    "id": 910,
    "temperature": 28.04,
    "humidity": 68.46,
    "lux": 10,
    "moisture": 0,
    "relay": 0,
    "receivedAt": "2026-01-29T22:29:41.847Z",
    "updatedAt": "2026-01-29T22:29:41.847Z"
  }
}
```

### POST /api/watering

Atualiza a configuração de irrigação.

**Body:**
```json
{
  "on": 20,
  "off": 80,
  "active": 1
}
```

- `on`: Porcentagem de umidade para ligar a bomba (0-100)
- `off`: Porcentagem de umidade para desligar a bomba (0-100)
- `active`: 1 para ativo, 0 para inativo

## ⚙️ Configurações

### Intervalo de atualização

Para alterar o intervalo de atualização dos dados (padrão: 5 segundos), edite a constante em `app/page.tsx`:

```typescript
const REFRESH_INTERVAL = 5000; // em milissegundos
```

### URL da API

Para alterar a URL da API, edite a constante em `app/page.tsx`:

```typescript
const API_BASE_URL = "http://localhost:3000/api";
```

### Quantidade de pontos no gráfico

Para alterar quantos pontos aparecem no gráfico (padrão: 20), edite em `app/page.tsx`:

```typescript
return newData.slice(-20); // últimos 20 pontos
```

## 🎨 Personalização

### Cores

As cores podem ser personalizadas no arquivo `app/globals.css` editando as variáveis CSS:

```css
:root {
  --primary: 221.2 83.2% 53.3%;
  --secondary: 210 40% 96.1%;
  /* ... outras cores */
}
```

### Tema escuro

O tema escuro já está configurado e pode ser ativado através das classes do Tailwind.

## 🐛 Solução de Problemas

### A aplicação não consegue se conectar à API

1. Verifique se a API está rodando
2. Confirme a URL da API em `app/page.tsx`
3. Verifique se há problemas de CORS

### Erro ao instalar dependências

1. Delete a pasta `node_modules`
2. Delete o arquivo `package-lock.json`
3. Execute `npm install` novamente

### Porta 3001 já está em uso

Execute em outra porta:
```bash
npm run dev -- -p 3002
```

## 📄 Licença

Este projeto é privado e destinado ao uso interno.

## 👨‍💻 Suporte

Para dúvidas ou problemas, entre em contato com a equipe de desenvolvimento.
