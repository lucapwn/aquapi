import { IsInt, IsNumber, Min, Max } from 'class-validator';

export class DeviceCommandDto {
  @IsNumber()
  @Min(0)
  @Max(100)
  on: number;

  @IsNumber()
  @Min(0)
  @Max(100)
  off: number;

  @IsInt()
  @Min(0)
  @Max(1)
  active: number;
}

export class DeviceCommandResponseDto {
  command: DeviceCommandDto;
  sentAt: Date;
}
