import { OnModuleInit, OnModuleDestroy } from '@nestjs/common';
import { PrismaService } from '../prisma/prisma.service';
import { DeviceDataResponseDto } from './dto/device-data.dto';
import { DeviceCommandDto, DeviceCommandResponseDto } from './dto/device-command.dto';
export declare class DeviceService implements OnModuleInit, OnModuleDestroy {
    private readonly prisma;
    private readonly logger;
    private mqttClient;
    private config;
    constructor(prisma: PrismaService);
    onModuleInit(): Promise<void>;
    onModuleDestroy(): Promise<void>;
    private connectMqtt;
    private subscribeToDataTopic;
    private handleIncomingMessage;
    private saveDeviceData;
    getLatestDeviceData(): Promise<DeviceDataResponseDto | null>;
    sendCommand(command: DeviceCommandDto): Promise<DeviceCommandResponseDto>;
}
