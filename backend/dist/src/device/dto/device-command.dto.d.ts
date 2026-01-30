export declare class DeviceCommandDto {
    on: number;
    off: number;
    active: number;
}
export declare class DeviceCommandResponseDto {
    command: DeviceCommandDto;
    sentAt: Date;
}
