import { DeviceService } from './device.service';
import { DeviceCommandDto } from './dto/device-command.dto';
export declare class DeviceController {
    private readonly deviceService;
    private readonly logger;
    constructor(deviceService: DeviceService);
    getLatestData(): Promise<{
        data: import("./dto/device-data.dto").DeviceDataResponseDto;
    }>;
    sendCommand(command: DeviceCommandDto): Promise<import("./dto/device-command.dto").DeviceCommandResponseDto>;
}
