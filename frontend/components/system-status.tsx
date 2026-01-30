import { Card, CardContent, CardHeader, CardTitle } from "@/components/ui/card";
import { Badge } from "@/components/ui/badge";
import { Activity, Droplets } from "lucide-react";
import { formatDistanceToNow } from "date-fns";
import { ptBR } from "date-fns/locale";

interface SystemStatusProps {
  lastUpdate: string;
  relayStatus: number;
}

export function SystemStatus({ lastUpdate, relayStatus }: SystemStatusProps) {
  const updateDate = new Date(lastUpdate);
  const isOnline = Date.now() - updateDate.getTime() < 3000; // Considera online se atualizou nos últimos 10s

  return (
    <Card>
      <CardHeader>
        <CardTitle className="text-sm font-medium">Status do Sistema</CardTitle>
      </CardHeader>
      <CardContent className="space-y-4">
        <div className="flex items-center justify-between">
          <div className="flex items-center gap-2">
            <Activity className="h-4 w-4 text-muted-foreground" />
            <span className="text-sm">Conexão</span>
          </div>
          <Badge className={`${isOnline ? "bg-green-500 hover:bg-green-500" : "bg-red-500 hover:bg-red-500"}`}>
            {isOnline ? "Online" : "Offline"}
          </Badge>
        </div>

        <div className="flex items-center justify-between">
          <div className="flex items-center gap-2">
            <Droplets className="h-4 w-4 text-muted-foreground" />
            <span className="text-sm">Bomba</span>
          </div>
          <Badge className={`${relayStatus ? "bg-green-500 hover:bg-green-500" : "bg-secondary hover:bg-secondary text-foreground/75"}`}>
            {relayStatus ? "Ligada" : "Desligada"}
          </Badge>
        </div>

        <div className="pt-2 border-t">
          <p className="text-xs text-muted-foreground">
            Última atualização:{" "}
            {formatDistanceToNow(updateDate, {
              addSuffix: true,
              locale: ptBR,
            })}
          </p>
          <p className="text-xs text-muted-foreground mt-1">
            {updateDate.toLocaleString("pt-BR")}
          </p>
        </div>
      </CardContent>
    </Card>
  );
}
