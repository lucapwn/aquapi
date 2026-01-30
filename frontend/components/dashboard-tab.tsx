"use client";

import { MetricCard } from "@/components/metric-card";
import { SensorChart } from "@/components/sensor-chart";
import { SystemStatus } from "@/components/system-status";
import { Thermometer, Droplet, Sun, Sprout } from "lucide-react";
import { SensorData, ChartDataPoint } from "@/types";

interface DashboardTabProps {
  sensorData: SensorData | null;
  chartData: ChartDataPoint[];
}

export function DashboardTab({ sensorData, chartData }: DashboardTabProps) {
  if (!sensorData) {
    return (
      <div className="flex items-center justify-center h-64">
        <p className="text-muted-foreground">Carregando dados...</p>
      </div>
    );
  }

  return (
    <div className="space-y-6">
      <div className="grid gap-4 md:grid-cols-2 lg:grid-cols-4">
        <MetricCard
          title="Temperatura"
          value={sensorData.temperature.toFixed(2)}
          unit="°C"
          icon={Thermometer}
          iconColor="text-red-500"
        />
        <MetricCard
          title="Umidade do Ar"
          value={sensorData.humidity.toFixed(2)}
          unit="%"
          icon={Droplet}
          iconColor="text-blue-500"
        />
        <MetricCard
          title="Luminosidade"
          value={sensorData.lux}
          unit="lux"
          icon={Sun}
          iconColor="text-yellow-500"
        />
        <MetricCard
          title="Umidade do Solo"
          value={sensorData.moisture}
          unit="%"
          icon={Sprout}
          iconColor="text-green-500"
        />
      </div>

      <SystemStatus
        lastUpdate={sensorData.updatedAt}
        relayStatus={sensorData.relay}
      />

      <SensorChart data={chartData} />
    </div>
  );
}
