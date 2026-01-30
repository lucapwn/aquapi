# 🚀 GUIA DE INSTALAÇÃO - AquaPi Dashboard

Siga este guia passo a passo para configurar e executar a aplicação.

## ✅ PASSO 1: Verificar Pré-requisitos

Antes de começar, certifique-se de ter instalado:

- **Node.js 18 ou superior**
  - Verifique: `node --version`
  - Download: https://nodejs.org/

- **npm (vem com Node.js)**
  - Verifique: `npm --version`

## 📦 PASSO 2: Instalar Dependências

Abra o terminal na pasta do projeto e execute:

```bash
npm install
```

Este comando irá instalar todas as dependências necessárias. Aguarde até a conclusão (pode levar alguns minutos).

## 🔌 PASSO 3: Configurar a API

A aplicação precisa se conectar à sua API backend. Por padrão, ela está configurada para:

```
http://localhost:3000/api
```

### Se sua API está em outro endereço:

1. Abra o arquivo `app/page.tsx`
2. Encontre a linha: `const API_BASE_URL = "http://localhost:3000/api";`
3. Altere para o endereço correto da sua API

### Certifique-se de que sua API possui os endpoints:

- **GET** `/api/sensors` - Retorna dados dos sensores
- **POST** `/api/watering` - Atualiza configuração

## ▶️ PASSO 4: Executar a Aplicação

### Modo Desenvolvimento (recomendado para testes)

```bash
npm run dev
```

A aplicação estará disponível em: **http://localhost:3001**

> **Nota:** Usamos a porta 3001 para não conflitar com a API na porta 3000.

### Se a porta 3001 estiver ocupada:

Execute em outra porta:
```bash
npm run dev -- -p 3002
```

## 🌐 PASSO 5: Acessar a Aplicação

1. Abra seu navegador
2. Acesse: **http://localhost:3001**
3. Você verá o dashboard do AquaPi

## 🧪 PASSO 6: Testar as Funcionalidades

### Tab Dashboard:
- Verifique se os dados dos sensores estão sendo exibidos
- Observe o gráfico sendo atualizado a cada 5 segundos
- Confirme o status da conexão

### Tab Irrigação:
- Clique em "Editar Configurações"
- Ajuste os valores dos sliders
- Ative/Desative o sistema
- Clique em "Salvar"

## 🏗️ PASSO 7: Build para Produção (Opcional)

Para criar uma versão otimizada para produção:

```bash
npm run build
npm run start
```

## ❌ Solução de Problemas Comuns

### Erro: "Cannot find module"
**Solução:** Execute novamente `npm install`

### Erro: "Port 3001 is already in use"
**Solução:** 
- Feche outros processos usando a porta
- OU execute em outra porta: `npm run dev -- -p 3002`

### Erro: "Failed to fetch"
**Solução:**
- Verifique se a API está rodando
- Confirme a URL da API em `app/page.tsx`
- Verifique problemas de CORS na API

### Dashboard não atualiza
**Solução:**
- Verifique se a API está respondendo corretamente
- Abra o console do navegador (F12) para ver erros
- Confirme que os dados da API estão no formato correto

### Erro de CORS
**Solução:** Configure o CORS no seu backend para aceitar requisições de:
- `http://localhost:3001`
- `http://localhost:3000`

## 📞 Suporte

Se encontrar problemas:

1. Verifique os logs no terminal
2. Abra o console do navegador (F12)
3. Consulte o README.md para mais detalhes
4. Verifique se todos os arquivos foram criados corretamente

## ✨ Próximos Passos

Após a instalação bem-sucedida:

1. Personalize as cores em `app/globals.css`
2. Ajuste o intervalo de atualização em `app/page.tsx`
3. Adicione mais funcionalidades conforme necessário
4. Configure para produção quando estiver pronto

---

**Pronto! Sua aplicação AquaPi está funcionando! 🎉**
