"use client";

import { WateringControl } from "@/components/watering-control";
import { Card, CardContent, CardHeader, CardTitle } from "@/components/ui/card";
import { Alert, AlertDescription } from "@/components/ui/alert";
import { Info, Droplets } from "lucide-react";
import { WateringConfig, SensorData } from "@/types";

interface IrrigationTabProps {
  wateringConfig: WateringConfig;
  sensorData: SensorData | null;
  onUpdateConfig: (config: WateringConfig) => Promise<void>;
}

export function IrrigationTab({
  wateringConfig,
  sensorData,
  onUpdateConfig,
}: IrrigationTabProps) {
  return (
    <div className="space-y-6">
      {sensorData && (
        <Card>
          <CardHeader>
            <CardTitle className="flex items-center gap-2">
              <Droplets className="h-5 w-5" />
              Status Atual
            </CardTitle>
          </CardHeader>
          <CardContent className="space-y-4">
            <div className="grid grid-cols-2 gap-4">
              <div>
                <p className="text-sm text-muted-foreground mb-1">
                  Umidade do Solo
                </p>
                <p className="text-2xl font-bold">{sensorData.moisture}%</p>
              </div>
              <div>
                <p className="text-sm text-muted-foreground mb-1">
                  Bomba de Água
                </p>
                <div className="flex items-center gap-2 mt-2">
                  <div
                    className={`h-3 w-3 rounded-full ${
                      sensorData.relay === 1
                        ? "bg-green-500 animate-pulse"
                        : "bg-red-500"
                    }`}
                  />
                  <p className="text-lg font-semibold">
                    {sensorData.relay === 1 ? "Ligada" : "Desligada"}
                  </p>
                </div>
              </div>
            </div>

            {wateringConfig.active === 1 && (
              <div className="pt-4 border-t">
                <p className="text-sm text-muted-foreground mb-2">
                  Próxima Ação Automática
                </p>
                {sensorData.moisture <= wateringConfig.on && sensorData.relay === 0 ? (
                  <p className="text-sm">
                    A bomba <span className="font-semibold">ligará</span> quando a
                    umidade atual ({sensorData.moisture}%) atingir o limite de
                    ativação ({wateringConfig.on}%).
                  </p>
                ) : sensorData.moisture >= wateringConfig.off &&
                  sensorData.relay === 1 ? (
                  <p className="text-sm">
                    A bomba <span className="font-semibold">desligará</span> pois
                    a umidade atual ({sensorData.moisture}%) atingiu o limite de
                    desativação ({wateringConfig.off}%)
                  </p>
                ) : (
                  <p className="text-sm">
                    Sistema em modo de <span className="font-semibold">espera</span> para 
                    a bomba ser desligada. Umidade dentro dos parâmetros.
                  </p>
                )}
              </div>
            )}

            {wateringConfig.active === 0 && (
              <div className="pt-4 border-t">
                <Alert>
                  <Info className="h-4 w-4" />
                  <AlertDescription>
                    O sistema automático está desativado. Ative-o para que a
                    irrigação funcione automaticamente.
                  </AlertDescription>
                </Alert>
              </div>
            )}
          </CardContent>
        </Card>
      )}

      <WateringControl config={wateringConfig} onUpdate={onUpdateConfig} />
    </div>
  );
}
