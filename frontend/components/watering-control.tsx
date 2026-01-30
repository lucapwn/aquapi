"use client";

import { useState } from "react";
import { Card, CardContent, CardHeader, CardTitle } from "@/components/ui/card";
import { Button } from "@/components/ui/button";
import {
  Dialog,
  DialogContent,
  DialogDescription,
  DialogFooter,
  DialogHeader,
  DialogTitle,
  DialogTrigger,
} from "@/components/ui/dialog";
import { Label } from "@/components/ui/label";
import { Switch } from "@/components/ui/switch";
import { Slider } from "@/components/ui/slider";
import { Settings2, Loader2 } from "lucide-react";
import { WateringConfig } from "@/types";

interface WateringControlProps {
  config: WateringConfig;
  onUpdate: (config: WateringConfig) => Promise<void>;
}

export function WateringControl({ config, onUpdate }: WateringControlProps) {
  const [open, setOpen] = useState(false);
  const [loading, setLoading] = useState(false);
  const [onValue, setOnValue] = useState(config.on);
  const [offValue, setOffValue] = useState(config.off);
  const [active, setActive] = useState(config.active === 1);
  const [error, setError] = useState<string | null>(null);

  const handleSave = async () => {
    setError(null);

    // Validação
    if (onValue >= offValue) {
      setError("O valor de ligar deve ser menor que o valor de desligar.");
      return;
    }

    if (onValue < 0 || offValue > 100) {
      setError("Os valores devem estar entre 0 e 100.");
      return;
    }

    setLoading(true);
    try {
      await onUpdate({
        on: onValue,
        off: offValue,
        active: active ? 1 : 0,
      });
      setOpen(false);
    } catch (err) {
      setError("Erro ao salvar configurações. Tente novamente.");
    } finally {
      setLoading(false);
    }
  };

  const handleOpenChange = (newOpen: boolean) => {
    if (!newOpen) {
      // Reset ao fechar
      setOnValue(config.on);
      setOffValue(config.off);
      setActive(config.active === 1);
      setError(null);
    }
    setOpen(newOpen);
  };

  return (
  <div className="space-y-4">
    <Card>
      <CardHeader>
        <CardTitle>Configurações de Irrigação</CardTitle>
      </CardHeader>

      <CardContent className="space-y-6">
        {/* Resumo */}
        <div className="grid grid-cols-2 gap-6">
          <div className="space-y-1">
            <p className="text-sm text-muted-foreground">Ligar em</p>
            <p className="text-2xl font-semibold">{config.on}%</p>
          </div>

          <div className="space-y-1">
            <p className="text-sm text-muted-foreground">Desligar em</p>
            <p className="text-2xl font-semibold">{config.off}%</p>
          </div>
        </div>

        {/* Status */}
        <div className="flex items-center justify-between rounded-lg border p-4">
          <div className="space-y-0.5">
            <p className="text-sm font-medium">Status da Irrigação</p>
            <p className="text-xs text-muted-foreground">
              {config.active === 1
                ? "Sistema ativo"
                : "Sistema inativo"}
            </p>
          </div>

          <span
            className={`px-3 py-1 rounded-full text-xs font-medium ${
              config.active === 1
                ? "bg-green-100 text-green-800 dark:bg-green-900 dark:text-green-100"
                : "bg-muted text-muted-foreground"
            }`}
          >
            {config.active === 1 ? "Ativo" : "Inativo"}
          </span>
        </div>

        {/* Modal */}
        <Dialog open={open} onOpenChange={handleOpenChange}>
          <DialogTrigger asChild>
            <Button className="w-full sm:w-auto gap-2">
              <Settings2 className="h-4 w-4" />
              Editar configurações
            </Button>
          </DialogTrigger>

          <DialogContent className="sm:max-w-[480px]">
            <DialogHeader>
              <DialogTitle>Configurar irrigação</DialogTitle>
              <DialogDescription>
                Defina os níveis de umidade para controle automático da irrigação.
              </DialogDescription>
            </DialogHeader>

            <div className="space-y-8 py-4">
              {/* Erro */}
              {error && (
                <div className="rounded-md border border-destructive/30 bg-destructive/10 px-4 py-3 text-sm text-destructive">
                  {error}
                </div>
              )}

              {/* Ligar */}
              <div className="space-y-3">
                <div className="flex items-center justify-between">
                  <Label htmlFor="on-value">Ligar irrigação</Label>
                  <span className="text-sm font-medium">
                    {onValue}%
                  </span>
                </div>

                <Slider
                  id="on-value"
                  min={0}
                  max={100}
                  step={1}
                  value={[onValue]}
                  onValueChange={(v) => setOnValue(v[0])}
                />

                <p className="text-xs text-muted-foreground">
                  A bomba liga quando a umidade do solo atinge este valor
                </p>
              </div>

              {/* Desligar */}
              <div className="space-y-3">
                <div className="flex items-center justify-between">
                  <Label htmlFor="off-value">Desligar irrigação</Label>
                  <span className="text-sm font-medium">
                    {offValue}%
                  </span>
                </div>

                <Slider
                  id="off-value"
                  min={0}
                  max={100}
                  step={1}
                  value={[offValue]}
                  onValueChange={(v) => setOffValue(v[0])}
                />

                <p className="text-xs text-muted-foreground">
                  A bomba desliga quando a umidade do solo atinge este valor
                </p>
              </div>

              {/* Switch */}
              <div className="rounded-lg border p-4 flex items-center justify-between">
                <div className="space-y-0.5">
                  <p className="text-sm font-medium">Ativar sistema</p>
                  <p className="text-xs text-muted-foreground">
                    {active
                      ? "Sistema irrigará automaticamente"
                      : "Sistema desativado"}
                  </p>
                </div>

                <Switch
                  checked={active}
                  onCheckedChange={setActive}
                />
              </div>
            </div>

            <DialogFooter className="gap-2">
              <Button
                variant="ghost"
                onClick={() => handleOpenChange(false)}
                disabled={loading}
              >
                Cancelar
              </Button>

              <Button onClick={handleSave} disabled={loading}>
                {loading && (
                  <Loader2 className="mr-2 h-4 w-4 animate-spin" />
                )}
                Salvar alterações
              </Button>
            </DialogFooter>
          </DialogContent>
        </Dialog>
      </CardContent>
    </Card>
  </div>
);
}
