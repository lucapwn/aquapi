export interface SensorData {
  id: number;
  temperature: number;
  humidity: number;
  lux: number;
  moisture: number;
  relay: number;
  receivedAt: string;
  updatedAt: string;
}

export interface SensorResponse {
  data: SensorData;
}

export interface WateringConfig {
  on: number;
  off: number;
  active: number;
}

export interface ChartDataPoint {
  time: string;
  temperature: number;
  humidity: number;
  moisture: number;
}
