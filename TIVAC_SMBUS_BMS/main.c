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

#define BMSREADDELAY 500000

char c[5];

void UARTIntHandler(void)
{
    uint32_t ui32Status;

    //
    // Get the interrrupt status.
    //
    ui32Status = UARTIntStatus(UART0_BASE, true);

    //
    // Clear the asserted interrupts.
    //
    UARTIntClear(UART0_BASE, ui32Status);

    //
    // Loop while there are characters in the receive FIFO.
    //
    while(UARTCharsAvail(UART0_BASE))
    {
        //
        // Read the next character from the UART and write it back to the UART.
        //
        UARTCharPutNonBlocking(UART0_BASE,
                               UARTCharGetNonBlocking(UART0_BASE));

        //
        // Blink the LED to show a character transfer is occuring.
        //
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);

        //
        // Delay for 1 millisecond.  Each SysCtlDelay is about 3 clocks.
        //
        SysCtlDelay(SysCtlClockGet() / (1000 * 3));

        //
        // Turn off the LED
        //
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);

    }
}



void UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
    //
    // Loop while there are more characters to send.
    //
    while(ui32Count--)
    {
        //
        // Write the next character to the UART.
        //
        UARTCharPutNonBlocking(UART0_BASE, *pui8Buffer++);
    }
}

int main(void)
{

    FPUEnable();
    FPULazyStackingEnable();
    //
    // Setup the system clock to run at 50 Mhz from PLL with crystal reference
    //
    SysCtlClockSet(SYSCTL_SYSDIV_1|SYSCTL_USE_OSC|SYSCTL_XTAL_16MHZ| SYSCTL_OSC_MAIN);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    IntMasterEnable();

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);

    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    UARTStdioConfig(0, 115200, 16000000);
    UARTprintf("UART Initialized \n\n\n");
    // Initialize Host SMBUs
    HostInit();
    //
    // Loop Forever
    //

    while(1)
    {

        Manufacturer_Access = ManufacturerAccess_Read();
        SysCtlDelay(BMSREADDELAY);
        Remaining_Capacity_Alarm = RemainingCapacityAlarm_Read();
        SysCtlDelay(BMSREADDELAY);
        Battery_Mode = BatteryMode_Read();
        SysCtlDelay(BMSREADDELAY);
        At_Rate = AtRate_Read();
        SysCtlDelay(BMSREADDELAY);
        At_Rate_Time_To_Full = AtRateTimeToFull_Read();
        SysCtlDelay(BMSREADDELAY);
        At_Rate_Time_To_Empty = AtRateTimeToEmpty_Read();
        SysCtlDelay(BMSREADDELAY);
        At_Rate_Ok = AtRateOK_Read();
        SysCtlDelay(BMSREADDELAY);
        Temperature = Temperature_Read();
        SysCtlDelay(BMSREADDELAY);
        Voltage = Voltage_Read();
        SysCtlDelay(BMSREADDELAY);
        Current = Current_Read();
        SysCtlDelay(BMSREADDELAY);
        Average_Current = AverageCurrent_Read();
        SysCtlDelay(BMSREADDELAY);
        Max_Error = MaxError_Read();
        SysCtlDelay(BMSREADDELAY);
        Relative_State_Of_Charge = RelativeStateOfCharge_Read();
        SysCtlDelay(BMSREADDELAY);
        Absolute_State_Of_Charge = AbsoluteStateOfCharge_Read();
        SysCtlDelay(BMSREADDELAY);
        Remaining_Capacity = RemainingCapacity_Read();
        SysCtlDelay(BMSREADDELAY);
        Full_Charge_Capacity = FullChargeCapacity_Read();
        SysCtlDelay(BMSREADDELAY);
        Runtime_To_Empty = RunTimeToEmpty_Read();
        SysCtlDelay(BMSREADDELAY);
        Average_Time_To_Empty = AverageTimeToEmpty_Read();
        SysCtlDelay(BMSREADDELAY);
        Average_Time_To_Full = AverageTimeToFull_Read();
        SysCtlDelay(BMSREADDELAY);
        //Charging_Current
        //Charging_Voltage;
        Battery_Status = BatteryStatus_Read();
        SysCtlDelay(BMSREADDELAY);
        Cycle_Count = CycleCount_Read();
        SysCtlDelay(BMSREADDELAY);
        Design_Capacity = DesignCapacity_Read();
        SysCtlDelay(BMSREADDELAY);
        Design_Voltage = DesignVoltage_Read();
        SysCtlDelay(BMSREADDELAY);
        Specification_Info = SpecificationInfo_Read();
        SysCtlDelay(BMSREADDELAY);
        Date = ManufactureDate_Read();
        SysCtlDelay(BMSREADDELAY);
        SerialNumber = SerialNumber_Read();

        /*SysCtlDelay(BMSREADDELAY);
        Charging_Current = ChargingCurrent();
        SysCtlDelay(BMSREADDELAY);
        Charging_Voltage = ChargingVoltage();
        SysCtlDelay(BMSREADDELAY);
        Cell_1_Voltage = Cell1Voltage();
        SysCtlDelay(BMSREADDELAY);
        Cell_2_Voltage = Cell2Voltage();
        SysCtlDelay(BMSREADDELAY);
        Cell_3_Voltage = Cell3Voltage();*/

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

        /*UARTgets(c, 3);
        if(c[0] == 'm')
        {
            ToggleMosfetCMD();
        }*/
        //
        // Delay for a bit
        //

        SysCtlDelay(16000000/3);


    }
}
