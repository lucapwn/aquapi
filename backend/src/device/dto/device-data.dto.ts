import { IsNumber, IsInt, Min, Max } from 'class-validator';

export class DeviceDataDto {
  @IsNumber()
  temperature: number;

  @IsNumber()
  @Min(0)
  @Max(100)
  humidity: number;

  @IsNumber()
  @Min(0)
  lux: number;

  @IsInt()
  @Min(0)
  @Max(1)
  moisture: number;

  @IsInt()
  @Min(0)
  @Max(1)
  relay: number;
}

export class DeviceDataResponseDto {
  id: number;
  temperature: number;
  humidity: number;
  lux: number;
  moisture: number;
  relay: number;
  receivedAt: Date;
  updatedAt: Date;
}
