"use strict";
var __createBinding = (this && this.__createBinding) || (Object.create ? (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    var desc = Object.getOwnPropertyDescriptor(m, k);
    if (!desc || ("get" in desc ? !m.__esModule : desc.writable || desc.configurable)) {
      desc = { enumerable: true, get: function() { return m[k]; } };
    }
    Object.defineProperty(o, k2, desc);
}) : (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    o[k2] = m[k];
}));
var __setModuleDefault = (this && this.__setModuleDefault) || (Object.create ? (function(o, v) {
    Object.defineProperty(o, "default", { enumerable: true, value: v });
}) : function(o, v) {
    o["default"] = v;
});
var __decorate = (this && this.__decorate) || function (decorators, target, key, desc) {
    var c = arguments.length, r = c < 3 ? target : desc === null ? desc = Object.getOwnPropertyDescriptor(target, key) : desc, d;
    if (typeof Reflect === "object" && typeof Reflect.decorate === "function") r = Reflect.decorate(decorators, target, key, desc);
    else for (var i = decorators.length - 1; i >= 0; i--) if (d = decorators[i]) r = (c < 3 ? d(r) : c > 3 ? d(target, key, r) : d(target, key)) || r;
    return c > 3 && r && Object.defineProperty(target, key, r), r;
};
var __importStar = (this && this.__importStar) || (function () {
    var ownKeys = function(o) {
        ownKeys = Object.getOwnPropertyNames || function (o) {
            var ar = [];
            for (var k in o) if (Object.prototype.hasOwnProperty.call(o, k)) ar[ar.length] = k;
            return ar;
        };
        return ownKeys(o);
    };
    return function (mod) {
        if (mod && mod.__esModule) return mod;
        var result = {};
        if (mod != null) for (var k = ownKeys(mod), i = 0; i < k.length; i++) if (k[i] !== "default") __createBinding(result, mod, k[i]);
        __setModuleDefault(result, mod);
        return result;
    };
})();
var __metadata = (this && this.__metadata) || function (k, v) {
    if (typeof Reflect === "object" && typeof Reflect.metadata === "function") return Reflect.metadata(k, v);
};
var DeviceService_1;
Object.defineProperty(exports, "__esModule", { value: true });
exports.DeviceService = void 0;
const common_1 = require("@nestjs/common");
const prisma_service_1 = require("../prisma/prisma.service");
const mqtt_config_1 = require("../config/mqtt.config");
const mqtt = __importStar(require("mqtt"));
let DeviceService = DeviceService_1 = class DeviceService {
    constructor(prisma) {
        this.prisma = prisma;
        this.logger = new common_1.Logger(DeviceService_1.name);
        this.config = (0, mqtt_config_1.mqttConfig)();
    }
    async onModuleInit() {
        this.connectMqtt();
    }
    async onModuleDestroy() {
        if (this.mqttClient) {
            this.mqttClient.end();
            this.logger.log('MQTT client disconnected.');
        }
    }
    connectMqtt() {
        this.logger.log(`Connecting to the MQTT broker: ${this.config.brokerUrl}`);
        this.mqttClient = mqtt.connect(this.config.brokerUrl, {
            clientId: this.config.clientId,
            clean: true,
            reconnectPeriod: 1000,
            connectTimeout: 30 * 1000,
        });
        this.mqttClient.on('connect', () => {
            this.logger.log('Connected to MQTT.');
            this.subscribeToDataTopic();
        });
        this.mqttClient.on('error', (error) => {
            this.logger.error(`Error connecting to MQTT: ${error.message}`);
        });
        this.mqttClient.on('reconnect', () => {
            this.logger.warn('Reconnecting to the MQTT broker.');
        });
        this.mqttClient.on('close', () => {
            this.logger.warn('MQTT connection closed.');
        });
        this.mqttClient.on('message', (topic, message) => {
            this.handleIncomingMessage(topic, message);
        });
    }
    subscribeToDataTopic() {
        this.mqttClient.subscribe(this.config.topicData, (error) => {
            if (error) {
                this.logger.error(`Error subscribing to the "${this.config.topicData}" topic: ${error.message}`);
            }
            else {
                this.logger.log(`Subscribed to the topic "${this.config.topicData}".`);
            }
        });
    }
    async handleIncomingMessage(topic, message) {
        try {
            const payload = JSON.parse(message.toString());
            this.logger.log(`Message received in the "${topic}" topic:`);
            this.logger.log(`${JSON.stringify(payload)}`);
            if (topic === this.config.topicData) {
                await this.saveDeviceData(payload);
            }
        }
        catch (error) {
            this.logger.error(`Error processing message: ${error.message}`);
        }
    }
    async saveDeviceData(data) {
        try {
            await this.prisma.deviceData.deleteMany({});
            await this.prisma.deviceData.create({
                data: {
                    temperature: data.temperature,
                    humidity: data.humidity,
                    lux: data.lux,
                    moisture: data.moisture,
                    relay: data.relay,
                },
            });
            this.logger.log('Data saved in the database.');
        }
        catch (error) {
            this.logger.error(`Error saving data: ${error.message}`);
            throw error;
        }
    }
    async getLatestDeviceData() {
        try {
            const data = await this.prisma.deviceData.findFirst({
                orderBy: {
                    receivedAt: 'desc',
                },
            });
            if (!data) {
                return null;
            }
            return {
                id: data.id,
                temperature: data.temperature,
                humidity: data.humidity,
                lux: data.lux,
                moisture: data.moisture,
                relay: data.relay,
                receivedAt: data.receivedAt,
                updatedAt: data.updatedAt,
            };
        }
        catch (error) {
            this.logger.error(`Error retrieving data: ${error.message}`);
            throw error;
        }
    }
    async sendCommand(command) {
        try {
            const payload = JSON.stringify(command);
            return new Promise((resolve, reject) => {
                this.mqttClient.publish(this.config.topicCommand, payload, { qos: 2 }, (error) => {
                    if (error) {
                        this.logger.error(`Error sending irrigation data: ${error.message}`);
                        reject(error);
                    }
                    else {
                        this.logger.log(`Command sent: ${payload}`);
                        resolve({
                            command,
                            sentAt: new Date(),
                        });
                    }
                });
            });
        }
        catch (error) {
            this.logger.error(`Error processing command: ${error.message}`);
            throw error;
        }
    }
};
exports.DeviceService = DeviceService;
exports.DeviceService = DeviceService = DeviceService_1 = __decorate([
    (0, common_1.Injectable)(),
    __metadata("design:paramtypes", [prisma_service_1.PrismaService])
], DeviceService);
//# sourceMappingURL=device.service.js.map