import { OnModuleInit, OnModuleDestroy } from '@nestjs/common';
export declare class PrismaService implements OnModuleInit, OnModuleDestroy {
    private prisma;
    private readonly logger;
    constructor();
    onModuleInit(): Promise<void>;
    onModuleDestroy(): Promise<void>;
    get deviceData(): import(".prisma/client").Prisma.DeviceDataDelegate<import("@prisma/client/runtime/library").DefaultArgs>;
    get $connect(): any;
    get $disconnect(): any;
}
