#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "driverlib/fpu.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "driverlib/can.h"
#include "BMS.h"

/**
 * Global Variables
 */
uint16_t Manufacturer_Access;
uint16_t Remaining_Capacity_Alarm;
uint16_t Battery_Mode;
uint16_t At_Rate;
uint16_t At_Rate_Time_To_Full;
uint16_t At_Rate_Time_To_Empty;
uint16_t At_Rate_Ok;
uint32_t Temperature;
uint16_t Voltage;
int16_t Current;
int16_t Average_Current;
uint16_t Max_Error;
uint16_t Relative_State_Of_Charge;
uint16_t Absolute_State_Of_Charge;
uint16_t Remaining_Capacity;
uint16_t Full_Charge_Capacity;
uint16_t Runtime_To_Empty;
uint16_t Average_Time_To_Empty;
uint16_t Average_Time_To_Full;
uint16_t Charging_Current;
uint16_t Charging_Voltage;
uint16_t Battery_Status;
uint16_t Cycle_Count;
uint16_t Design_Capacity;
uint16_t Design_Voltage;
uint16_t Specification_Info;
SMB_tDate Date;
uint16_t SerialNumber;


int main(void)
{

    // Enable FPU
    FPUEnable();
    FPULazyStackingEnable();
    // Setup the system clock to run at 50MHz from PLL with crystal reference
    SysCtlClockSet(SYSCTL_SYSDIV_1|SYSCTL_USE_OSC|SYSCTL_XTAL_16MHZ| SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); // Enable clock for GPIOA (Used by UART0)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0); // Enable clock for UART0 peripheral
    GPIOPinConfigure(GPIO_PA0_U0RX); // Configure GPIO PA0 as UART0 Rx
    GPIOPinConfigure(GPIO_PA1_U0TX); // Configure GPIO PA1 as UART0 Tx
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC); // Configure clock source for UART0
    IntMasterEnable(); // Enable global interrupt
    UARTStdioConfig(0, 115200, 16000000); // Configure UART0 with baud rate 115200
    UARTprintf("UART Initialized \n\n\n"); // Print a test text
    HostInit(); // Initialize Host SMBUs

    //
    // Loop Forever
    //

    while(1)
    {

        // Read the data from BMS registers
        Manufacturer_Access = ManufacturerAccess_Read();
        Remaining_Capacity_Alarm = RemainingCapacityAlarm_Read();
        Battery_Mode = BatteryMode_Read();
        At_Rate = AtRate_Read();
        At_Rate_Time_To_Full = AtRateTimeToFull_Read();
        At_Rate_Time_To_Empty = AtRateTimeToEmpty_Read();
        At_Rate_Ok = AtRateOK_Read();
        Temperature = Temperature_Read();
        Voltage = Voltage_Read();
        Current = Current_Read();
        Average_Current = AverageCurrent_Read();
        Max_Error = MaxError_Read();
        Relative_State_Of_Charge = RelativeStateOfCharge_Read();
        Absolute_State_Of_Charge = AbsoluteStateOfCharge_Read();
        Remaining_Capacity = RemainingCapacity_Read();
        Full_Charge_Capacity = FullChargeCapacity_Read();
        Runtime_To_Empty = RunTimeToEmpty_Read();
        Average_Time_To_Empty = AverageTimeToEmpty_Read();
        Average_Time_To_Full = AverageTimeToFull_Read();
        //Charging_Current
        //Charging_Voltage;
        Battery_Status = BatteryStatus_Read();
        Cycle_Count = CycleCount_Read();
        Design_Capacity = DesignCapacity_Read();
        Design_Voltage = DesignVoltage_Read();
        Specification_Info = SpecificationInfo_Read();
        Date = ManufactureDate_Read();
        SerialNumber = SerialNumber_Read();

        //Charging_Current = ChargingCurrent();
        //Charging_Voltage = ChargingVoltage();
        //Cell_1_Voltage = Cell1Voltage();
        //Cell_2_Voltage = Cell2Voltage();
        //Cell_3_Voltage = Cell3Voltage();

        // Print the data
        UARTprintf("BQ4050 SBS Commands Example \n");
        UARTprintf("Manufacturer Access: 0x%04X \n",Manufacturer_Access);
        UARTprintf("Remaining Capacity Alarm: %d mAh\n",Remaining_Capacity_Alarm);
        UARTprintf("Battery Mode: 0x%04X \n",Battery_Mode);
        UARTprintf("At Rate: %d mA \n",At_Rate);
        UARTprintf("At Rate Time To Full: %d minute(s) \n",At_Rate_Time_To_Full);
        UARTprintf("At Rate Time To Empty: %d minute(s) \n",At_Rate_Time_To_Empty);
        UARTprintf("At Rate OK: %d \n",At_Rate_Ok);
        UARTprintf("Temperature: %d Degree C \n",Temperature);
        UARTprintf("Voltage: %d mV \n",Voltage);
        UARTprintf("Current: %d mA \n",Current);
        UARTprintf("Average Current: %d mA \n",Average_Current);
        UARTprintf("Max Error: %d %% \n",Max_Error);
        UARTprintf("Relative State Of Charge: %d %% \n",Relative_State_Of_Charge);
        UARTprintf("Absolute State Of Charge: %d %% \n",Absolute_State_Of_Charge);
        UARTprintf("Remaining Capacity: %d mAh \n",Remaining_Capacity);
        UARTprintf("Full Charge Capacity: %d mAh \n",Full_Charge_Capacity);
        UARTprintf("Run Time To Empty: %d min \n",Runtime_To_Empty);
        UARTprintf("Average Time To Empty: %d min \n",Average_Time_To_Empty);
        UARTprintf("Average Time To Full: %d min \n",Average_Time_To_Full);
        //UARTprintf("Charging Voltage: %d mV \n",Charging_Voltage);
        //UARTprintf("Charging Current: %d mA \n",Charging_Current);
        UARTprintf("Battery Status: 0x%04X \n",Battery_Status);
        UARTprintf("Cycle Count: %d \n",Cycle_Count);
        UARTprintf("Design Capacity: %d mAh \n",Design_Capacity);
        UARTprintf("Design Voltage: %d mV \n",Design_Voltage);
        UARTprintf("Manufacture Date: %02d\\%02d\\%d\n",Date.Day,Date.Month,Date.Year);
        UARTprintf("Serial Number: 0x%04X \n",SerialNumber);
        //UARTprintf("Cell 1 Voltage: %d mV \n",Cell_1_Voltage);
        //UARTprintf("Cell 2 Voltage: %d mV \n",Cell_2_Voltage);
        //UARTprintf("Cell 3 Voltage: %d mV \n",Cell_3_Voltage);
        //UARTprintf("Safety Status: 0x%X \n",safetystatus);
        UARTprintf("\n");
        SysCtlDelay(16000000/3); // Delay for 1 Second
    }
}
