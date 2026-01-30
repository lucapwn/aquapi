import { Card, CardContent, CardHeader, CardTitle } from "@/components/ui/card";
import { LucideIcon } from "lucide-react";

interface MetricCardProps {
  title: string;
  value: string | number;
  unit: string;
  icon: LucideIcon;
  iconColor: string;
}

export function MetricCard({
  title,
  value,
  unit,
  icon: Icon,
  iconColor,
}: MetricCardProps) {
  return (
    <Card>
      <CardHeader className="flex flex-row items-center justify-between space-y-0 pb-2">
        <CardTitle className="text-sm font-medium">{title}</CardTitle>
        <Icon className={`h-5 w-5 ${iconColor}`} />
      </CardHeader>
      <CardContent className="pt-0">
        <div className="flex items-baseline gap-1">
          <span className="text-2xl font-bold leading-none">
            {value}
          </span>
          <span className="text-sm font-normal text-muted-foreground">
            {unit}
          </span>
        </div>
      </CardContent>
    </Card>
  );
}
