import { Controller, Get, Post, Body, HttpStatus, HttpException, Logger } from '@nestjs/common';
import { DeviceService } from './device.service';
import { DeviceCommandDto } from './dto/device-command.dto';

@Controller('api')
export class DeviceController {
  private readonly logger = new Logger(DeviceController.name);

  constructor(private readonly deviceService: DeviceService) {}

  @Get('sensors')
  async getLatestData() {
    try {
      const data = await this.deviceService.getLatestDeviceData();

      if (!data) {
        throw new HttpException(
          'No data available.',
          HttpStatus.NOT_FOUND,
        );
      }

      this.logger.log('Data successfully retrieved.');

      return {
        data,
      };
    } catch (error) {
      this.logger.error(`Error retrieving data.`);
      
      if (error instanceof HttpException) {
        throw error;
      }
      
      throw new HttpException(
        'Error retrieving device data.',
        HttpStatus.INTERNAL_SERVER_ERROR,
      );
    }
  }

  @Post('watering')
  async sendCommand(@Body() command: DeviceCommandDto) {
    try {
      this.logger.log(`Command received: ${JSON.stringify(command)}`);

      const result = await this.deviceService.sendCommand(command);

      return result;
    } catch (error) {
      this.logger.error(`Error sending command.`);
      
      throw new HttpException(
        'Error sending command to the device.',
        HttpStatus.INTERNAL_SERVER_ERROR,
      );
    }
  }
}
