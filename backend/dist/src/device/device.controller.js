"use strict";
var __decorate = (this && this.__decorate) || function (decorators, target, key, desc) {
    var c = arguments.length, r = c < 3 ? target : desc === null ? desc = Object.getOwnPropertyDescriptor(target, key) : desc, d;
    if (typeof Reflect === "object" && typeof Reflect.decorate === "function") r = Reflect.decorate(decorators, target, key, desc);
    else for (var i = decorators.length - 1; i >= 0; i--) if (d = decorators[i]) r = (c < 3 ? d(r) : c > 3 ? d(target, key, r) : d(target, key)) || r;
    return c > 3 && r && Object.defineProperty(target, key, r), r;
};
var __metadata = (this && this.__metadata) || function (k, v) {
    if (typeof Reflect === "object" && typeof Reflect.metadata === "function") return Reflect.metadata(k, v);
};
var __param = (this && this.__param) || function (paramIndex, decorator) {
    return function (target, key) { decorator(target, key, paramIndex); }
};
var DeviceController_1;
Object.defineProperty(exports, "__esModule", { value: true });
exports.DeviceController = void 0;
const common_1 = require("@nestjs/common");
const device_service_1 = require("./device.service");
const device_command_dto_1 = require("./dto/device-command.dto");
let DeviceController = DeviceController_1 = class DeviceController {
    constructor(deviceService) {
        this.deviceService = deviceService;
        this.logger = new common_1.Logger(DeviceController_1.name);
    }
    async getLatestData() {
        try {
            const data = await this.deviceService.getLatestDeviceData();
            if (!data) {
                throw new common_1.HttpException('No data available.', common_1.HttpStatus.NOT_FOUND);
            }
            this.logger.log('Data successfully retrieved.');
            return {
                data,
            };
        }
        catch (error) {
            this.logger.error(`Error retrieving data.`);
            if (error instanceof common_1.HttpException) {
                throw error;
            }
            throw new common_1.HttpException('Error retrieving device data.', common_1.HttpStatus.INTERNAL_SERVER_ERROR);
        }
    }
    async sendCommand(command) {
        try {
            this.logger.log(`Command received: ${JSON.stringify(command)}`);
            const result = await this.deviceService.sendCommand(command);
            return result;
        }
        catch (error) {
            this.logger.error(`Error sending command.`);
            throw new common_1.HttpException('Error sending command to the device.', common_1.HttpStatus.INTERNAL_SERVER_ERROR);
        }
    }
};
exports.DeviceController = DeviceController;
__decorate([
    (0, common_1.Get)('sensors'),
    __metadata("design:type", Function),
    __metadata("design:paramtypes", []),
    __metadata("design:returntype", Promise)
], DeviceController.prototype, "getLatestData", null);
__decorate([
    (0, common_1.Post)('watering'),
    __param(0, (0, common_1.Body)()),
    __metadata("design:type", Function),
    __metadata("design:paramtypes", [device_command_dto_1.DeviceCommandDto]),
    __metadata("design:returntype", Promise)
], DeviceController.prototype, "sendCommand", null);
exports.DeviceController = DeviceController = DeviceController_1 = __decorate([
    (0, common_1.Controller)('api'),
    __metadata("design:paramtypes", [device_service_1.DeviceService])
], DeviceController);
//# sourceMappingURL=device.controller.js.map