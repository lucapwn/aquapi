export interface MqttConfig {
    brokerUrl: string;
    clientId: string;
    topicData: string;
    topicCommand: string;
}
export declare const mqttConfig: () => MqttConfig;
