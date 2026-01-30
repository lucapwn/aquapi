-- CreateTable
CREATE TABLE "device_data" (
    "id" SERIAL NOT NULL,
    "temperature" DOUBLE PRECISION NOT NULL,
    "humidity" DOUBLE PRECISION NOT NULL,
    "lux" DOUBLE PRECISION NOT NULL,
    "moisture" INTEGER NOT NULL,
    "relay" INTEGER NOT NULL,
    "receivedAt" TIMESTAMP(3) NOT NULL DEFAULT CURRENT_TIMESTAMP,
    "updatedAt" TIMESTAMP(3) NOT NULL,

    CONSTRAINT "device_data_pkey" PRIMARY KEY ("id")
);
