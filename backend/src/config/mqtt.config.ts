export interface MqttConfig {
  brokerUrl: string;
  clientId: string;
  topicData: string;
  topicCommand: string;
}

export const mqttConfig = (): MqttConfig => ({
  brokerUrl: process.env.MQTT_BROKER_URL || 'mqtt://broker.hivemq.com',
  clientId: process.env.MQTT_CLIENT_ID || 'aquapi-backend-client',
  topicData: process.env.MQTT_TOPIC_DATA || 'message/server',
  topicCommand: process.env.MQTT_TOPIC_COMMAND || 'message/device',
});
