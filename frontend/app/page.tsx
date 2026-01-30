"use client";

import { useState, useEffect, useCallback } from "react";
import { Tabs, TabsContent, TabsList, TabsTrigger } from "@/components/ui/tabs";
import { DashboardTab } from "@/components/dashboard-tab";
import { IrrigationTab } from "@/components/irrigation-tab";
import { Droplets } from "lucide-react";
import { SensorData, WateringConfig, ChartDataPoint } from "@/types";
import { format } from "date-fns";

const API_BASE_URL = "http://localhost:3000/api";
const REFRESH_INTERVAL = 800; // ms

export default function Home() {
  const [sensorData, setSensorData] = useState<SensorData | null>(null);
  const [chartData, setChartData] = useState<ChartDataPoint[]>([]);
  const [wateringConfig, setWateringConfig] = useState<WateringConfig>({
    on: 20,
    off: 80,
    active: 1,
  });
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);

  const fetchSensorData = useCallback(async () => {
    try {
      const response = await fetch(`${API_BASE_URL}/sensors`);
      if (!response.ok) {
        throw new Error("Erro ao buscar dados dos sensores.");
      }
      const result = await response.json();
      setSensorData(result.data);

      // Atualiza o gráfico com os novos dados
      setChartData((prevData) => {
        const newDataPoint: ChartDataPoint = {
          time: format(new Date(result.data.updatedAt), "HH:mm:ss"),
          temperature: result.data.temperature,
          humidity: result.data.humidity,
          moisture: result.data.moisture,
        };

        const newData = [...prevData, newDataPoint];
        // Mantém apenas os últimos 10 dados
        return newData.slice(-10);
      });

      setError(null);
      setLoading(false);
    } catch (err) {
      setError(
        err instanceof Error ? err.message : "Erro ao carregar dados."
      );
      setLoading(false);
    }
  }, []);

  const updateWateringConfig = async (config: WateringConfig) => {
    try {
      const response = await fetch(`${API_BASE_URL}/watering`, {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify(config),
      });

      if (!response.ok) {
        throw new Error("Erro ao atualizar configuração.");
      }

      setWateringConfig(config);
    } catch (err) {
      throw new Error(
        err instanceof Error
          ? err.message
          : "Erro ao atualizar configuração."
      );
    }
  };

  useEffect(() => {
    // Busca inicial
    fetchSensorData();

    // Configura intervalo para atualização automática
    const interval = setInterval(fetchSensorData, REFRESH_INTERVAL);

    return () => clearInterval(interval);
  }, [fetchSensorData]);

  if (loading) {
    return (
      <div className="min-h-screen flex items-center justify-center bg-background">
        <div className="text-center space-y-4">
          <Droplets className="h-12 w-12 animate-pulse mx-auto text-primary" />
          <p className="text-muted-foreground">Carregando AquaPi...</p>
        </div>
      </div>
    );
  }

  return (
    <div className="min-h-screen bg-background">
      <div className="container mx-auto py-6 px-4 max-w-7xl">
        <div className="mb-8">
          <div className="flex items-center gap-3 mb-2">
            <Droplets className="h-8 w-8 text-primary" />
            <h1 className="text-3xl font-bold">AquaPi</h1>
          </div>
          <p className="text-muted-foreground">
            Aplicação Inteligente de Irrigação IoT
          </p>
        </div>

        {error && (
          <div className="mb-6 bg-destructive/10 text-destructive p-4 rounded-lg">
            {error}
          </div>
        )}

        <Tabs defaultValue="dashboard" className="space-y-6">
          <TabsList className="grid w-full max-w-md grid-cols-2">
            <TabsTrigger value="dashboard">Dashboard</TabsTrigger>
            <TabsTrigger value="irrigation">Configuração</TabsTrigger>
          </TabsList>

          <TabsContent value="dashboard" className="space-y-6">
            <DashboardTab sensorData={sensorData} chartData={chartData} />
          </TabsContent>

          <TabsContent value="irrigation" className="space-y-6">
            <IrrigationTab
              wateringConfig={wateringConfig}
              sensorData={sensorData}
              onUpdateConfig={updateWateringConfig}
            />
          </TabsContent>
        </Tabs>
      </div>
    </div>
  );
}
