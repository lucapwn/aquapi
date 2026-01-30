import { Injectable, OnModuleInit, OnModuleDestroy, Logger } from '@nestjs/common';
import { PrismaService } from '../prisma/prisma.service';
import { DeviceDataDto, DeviceDataResponseDto } from './dto/device-data.dto';
import { DeviceCommandDto, DeviceCommandResponseDto } from './dto/device-command.dto';
import { mqttConfig } from '../config/mqtt.config';
import * as mqtt from 'mqtt';

@Injectable()
export class DeviceService implements OnModuleInit, OnModuleDestroy {
  private readonly logger = new Logger(DeviceService.name);
  private mqttClient: mqtt.MqttClient;
  private config = mqttConfig();

  constructor(private readonly prisma: PrismaService) {}

  async onModuleInit() {
    this.connectMqtt();
  }

  async onModuleDestroy() {
    if (this.mqttClient) {
      this.mqttClient.end();
      this.logger.log('MQTT client disconnected.');
    }
  }

  private connectMqtt() {
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

  private subscribeToDataTopic() {
    this.mqttClient.subscribe(this.config.topicData, (error) => {
      if (error) {
        this.logger.error(`Error subscribing to the "${this.config.topicData}" topic: ${error.message}`);
      } else {
        this.logger.log(`Subscribed to the topic "${this.config.topicData}".`);
      }
    });
  }

  private async handleIncomingMessage(topic: string, message: Buffer) {
    try {
      const payload = JSON.parse(message.toString());
      this.logger.log(`Message received in the "${topic}" topic:`);
      this.logger.log(`${JSON.stringify(payload)}`);

      if (topic === this.config.topicData) {
        await this.saveDeviceData(payload);
      }
    } catch (error) {
      this.logger.error(`Error processing message: ${error.message}`);
    }
  }

  private async saveDeviceData(data: DeviceDataDto): Promise<void> {
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
    } catch (error) {
      this.logger.error(`Error saving data: ${error.message}`);
      throw error;
    }
  }

  async getLatestDeviceData(): Promise<DeviceDataResponseDto | null> {
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
    } catch (error) {
      this.logger.error(`Error retrieving data: ${error.message}`);
      throw error;
    }
  }

  async sendCommand(command: DeviceCommandDto): Promise<DeviceCommandResponseDto> {
    try {
      const payload = JSON.stringify(command);
      
      return new Promise((resolve, reject) => {
        this.mqttClient.publish(
          this.config.topicCommand,
          payload,
          { qos: 2 },
          (error) => {
            if (error) {
              this.logger.error(`Error sending irrigation data: ${error.message}`);
              reject(error);
            } else {
              this.logger.log(`Command sent: ${payload}`);
              resolve({
                command,
                sentAt: new Date(),
              });
            }
          },
        );
      });
    } catch (error) {
      this.logger.error(`Error processing command: ${error.message}`);
      throw error;
    }
  }
}
