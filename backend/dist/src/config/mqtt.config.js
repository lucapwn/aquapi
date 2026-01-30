"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.mqttConfig = void 0;
const mqttConfig = () => ({
    brokerUrl: process.env.MQTT_BROKER_URL || 'mqtt://broker.hivemq.com',
    clientId: process.env.MQTT_CLIENT_ID || 'aquapi-backend-client',
    topicData: process.env.MQTT_TOPIC_DATA || 'message/server',
    topicCommand: process.env.MQTT_TOPIC_COMMAND || 'message/device',
});
exports.mqttConfig = mqttConfig;
//# sourceMappingURL=mqtt.config.js.map