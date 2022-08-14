/*
 * BMS.h
 *
 *  Created on: Jul 20, 2022
 *      Author: Admin-PC
 */

#ifndef BMS_H_
#define BMS_H_

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
#include "utils/smbus.h"


#define MAX_SMB_BLOCK_SIZE 12u

#define DISABLE_PEC // Disable Packer Error Checking option (PEC)

void HostInit(void);
void SMBusMasterIntHandler(void);


/**
 * Register map for SBS commands
 * See: http://sbs-forum.org/specs/sbdat110.pdf for more details (Page 51)
 *      http://sbs-forum.org/specs/
 * this table should work with any BMS that comply with Smart Battery Data Specification
 *
 */
#define SBS_REG_ManufacturerAccess                          0x00u       // R/W  ,   Word size
#define SBS_REG_RemainingCapacityAlarm                      0x01u       // R/W  ,   Word size
#define SBS_REG_RemainingTimeAlarm                          0x02u       // R/W  ,   Word size
#define SBS_REG_BatteryMode                                 0x03u       // R/W  ,   Word size
#define SBS_REG_AtRate                                      0x04u       // R/W  ,   Word size
#define SBS_REG_AtRateTimeToFull                            0x05u       // R/   ,   Word size
#define SBS_REG_AtRateTimeToEmpty                           0x06u       // R/   ,   Word size
#define SBS_REG_AtRateOK                                    0x07u       // R/   ,   Word size
#define SBS_REG_Temperature                                 0x08u       // R/   ,   Word size   ,   (0.1°K)
#define SBS_REG_Voltage                                     0x09u       // R/   ,   Word size   ,   (mV)
#define SBS_REG_Current                                     0x0Au       // R/   ,   Word size   ,   (mA)
#define SBS_REG_AverageCurrent                              0x0Bu       // R/   ,   Word size   ,   (mA)
#define SBS_REG_MaxError                                    0x0Cu       // R/   ,   Word size   ,   (percent)
#define SBS_REG_RelativeStateOfCharge                       0x0Du       // R/   ,   Word size   ,   (percent)
#define SBS_REG_AbsoluteStateOfCharge                       0x0Eu       // R/   ,   Word size   ,   (percent)
#define SBS_REG_RemainingCapacity                           0x0Fu       // R/   ,   Word size   ,   (mAh or 10mWh)
#define SBS_REG_FullChargeCapacity                          0x10u       // R/   ,   Word size   ,   (mAh or 10mWh)
#define SBS_REG_RunTimeToEmpty                              0x11u       // R/   ,   Word size   ,   (minutes)
#define SBS_REG_AverageTimeToEmpty                          0x12u       // R/   ,   Word size   ,   (minutes)
#define SBS_REG_AverageTimeToFull                           0x13u       // R/   ,   Word size   ,   (minutes)
#define SBS_REG_ChargingCurrent                             0x14u       // R/   ,   Word size   ,   (mA)
#define SBS_REG_ChargingVoltage                             0x15u       // R/   ,   Word size   ,   (mV)
#define SBS_REG_BatteryStatus                               0x16u       // R/   ,   Word size   ,   (bit flags, see the user guide)
#define SBS_REG_CycleCount                                  0x17u       // R/   ,   Word size   ,   (count)
#define SBS_REG_DesignCapacity                              0x18u       // R/   ,   Word size   ,   (mAh or 10mWh)
#define SBS_REG_DesignVoltage                               0x19u       // R/   ,   Word size   ,   (mV)
#define SBS_REG_SpecificationInfo                           0x1Au       // R/   ,   unsigned int
#define SBS_REG_ManufactureDate                             0x1Bu       // R/   ,   unsigned int
#define SBS_REG_SerialNumber                                0x1Cu       // R/   ,   number
// RESERVED                                                 0x1D - 0x1F
#define SBS_REG_ManufacturerName                            0x20u       // R/   ,   string
#define SBS_REG_DeviceName                                  0x21u       // R/   ,   string
#define SBS_REG_DeviceChemistry                             0x22u       // R/   ,   string
#define SBS_REG_ManufacturerData                            0x23u       // R/   ,   data
// RESERVED                                                 0x25 - 0x2E
#define SBS_REG_OptionalMfgFunction5                        0x2Fu       // R/W  ,   data
// RESERVED                                                 0x30 - 0x3B
#define SBS_REG_OptionalMfgFunction4                        0x3Cu       // R/W  ,   data

// Specific Registers for TI
#define SBS_REG_CellVoltage4                                0x3Cu       // R/   ,   Word size   ,   (mV)
#define SBS_REG_CellVoltage3                                0x3Du       // R/   ,   Word size   ,   (mV)
#define SBS_REG_CellVoltage2                                0x3Eu       // R/   ,   Word size   ,   (mV)
#define SBS_REG_CellVoltage1                                0x3Fu       // R/   ,   Word size   ,   (mV)



/**
 * @details This Date typedef for the ManufactureDate() function
 * @typedef
 */
typedef struct{
    uint16_t Day;
    uint16_t Month;
    uint16_t Year;
}SMB_tDate;



/**
 * This section for Function decleration
 */



/**
 * This function is optional and its meaning is implementation specific. It may be used by a battery
 * manufacturer or silicon supplier to return specific version information, internal calibration information, or
 * some other manufacturer specific function. There is no implied or required use for this function and
 * therefore it may be used for multiple purposes. The only requirement is the data protocol listed below: read
 * word or write word.
 * @details The ManufacturerAccess() function's purpose is manufacturer specific. No functional requirement is
 * implied although example uses are mentioned in this text.
 * Address:         0x00
 * @return          word -- Content determined by the Smart Battery's manufacturer
 */
uint16_t ManufacturerAccess_Read(void);



/**
 * This function is optional and its meaning is implementation specific. It may be used by a battery
 * manufacturer or silicon supplier to return specific version information, internal calibration information, or
 * some other manufacturer specific function. There is no implied or required use for this function and
 * therefore it may be used for multiple purposes. The only requirement is the data protocol listed below: read
 * word or write word.
 * @details The ManufacturerAccess() function's purpose is manufacturer specific. No functional requirement is
 * implied although example uses are mentioned in this text.
 * Address:         0x00
 * @return          word -- Content determined by the Smart Battery's manufacturer
 */
void ManufacturerAccess_Write(uint16_t word);



/**
 * Sets or gets the Low Capacity alarm threshold value. Whenever the RemainingCapacity() falls below the
 * Low Capacity value, the Smart Battery sends AlarmWarning() messages to the SMBus Host with the
 * REMAINING_CAPACITY_ALARM bit set. A Low Capacity value of 0 disables this alarm.
 * (If the ALARM_MODE bit is set in BatteryMode() then the AlarmWarning() message is disabled for a set
 * period of time. See the BatteryMode() function for further information.)
 * The Low Capacity value is set to 10% of design capacity at time of manufacture. The Low Capacity value
 * will remain unchanged until altered by the RemainingCapacityAlarm() function. The Low Capacity value
 * may be expressed in either capacity (mAh) or power (10mWh) depending on the setting of the
 * BatteryMode()'s CAPACITY_MODE bit (see BatteryMode()).
 * @details The RemainingCapacityAlarm() function can be used by systems to indicate a first level near end of
 * discharge state. Since the alarm and the RemainingCapacity() value itself are expressed at C/5 or P/5
 * discharge rates, the value may not directly correspond to the actual present discharge rate. Although this
 * provides a finely controlled alarm set-point, the RemainingTimeAlarm() and related time functions are
 * better suited to for indicating at which point a system should transition into a suspend or hibernate state.
 * The Low Capacity value can be read to verify the value in use by the Smart Battery's Low Capacity alarm.
 * Address:         0x01
 * @return          unsigned int -- value below which Low Capacity messages will be sent
 * ---------------------------------------------------------------------------
 * |                             Battery Mode                                |
 * ---------------------------------------------------------------------------
 * |               |    CAPACITY_MODE bit = 0   |   CAPACITY_MODE bit = 1    |
 * ---------------------------------------------------------------------------
 * |    units      |         mAh @ C/5          |         10mWh @ P/5        |
 * ---------------------------------------------------------------------------
 * |    Range      |      0 to 65,535 mAh       |     0 to 65,535 10mWh      |
 * ---------------------------------------------------------------------------
 * | Granularity   |                     not applicable                      |
 *  * ------------------------------------------------------------------------
 * |    Accuracy   |                     see RemainingCapacity()             |
 * ---------------------------------------------------------------------------
 */
uint16_t RemainingCapacityAlarm_Read(void);



/**
 * Sets or gets the Low Capacity alarm threshold value. Whenever the RemainingCapacity() falls below the
 * Low Capacity value, the Smart Battery sends AlarmWarning() messages to the SMBus Host with the
 * REMAINING_CAPACITY_ALARM bit set. A Low Capacity value of 0 disables this alarm.
 * (If the ALARM_MODE bit is set in BatteryMode() then the AlarmWarning() message is disabled for a set
 * period of time. See the BatteryMode() function for further information.)
 * The Low Capacity value is set to 10% of design capacity at time of manufacture. The Low Capacity value
 * will remain unchanged until altered by the RemainingCapacityAlarm() function. The Low Capacity value
 * may be expressed in either capacity (mAh) or power (10mWh) depending on the setting of the
 * BatteryMode()'s CAPACITY_MODE bit (see BatteryMode()).
 * @details The RemainingCapacityAlarm() function can be used by systems to indicate a first level near end of
 * discharge state. Since the alarm and the RemainingCapacity() value itself are expressed at C/5 or P/5
 * discharge rates, the value may not directly correspond to the actual present discharge rate. Although this
 * provides a finely controlled alarm set-point, the RemainingTimeAlarm() and related time functions are
 * better suited to for indicating at which point a system should transition into a suspend or hibernate state.
 * The Low Capacity value can be read to verify the value in use by the Smart Battery's Low Capacity alarm.
 * SMBus Protocol: Read or Write Word
 * Address:         0x01
 * Input/Output:    unsigned int -- value below which Low Capacity messages will be sent
 * ---------------------------------------------------------------------------
 * |                             Battery Mode                                |
 * |-------------------------------------------------------------------------|
 * |               |    CAPACITY_MODE bit = 0   |   CAPACITY_MODE bit = 1    |
 * |-------------------------------------------------------------------------|
 * |    units      |         mAh @ C/5          |         10mWh @ P/5        |
 * |-------------------------------------------------------------------------|
 * |    Range      |      0 to 65,535 mAh       |     0 to 65,535 10mWh      |
 * |-------------------------------------------------------------------------|
 * | Granularity   |                     not applicable                      |
 * |-------------------------------------------------------------------------|
 * |    Accuracy   |                     see RemainingCapacity()             |
 * ---------------------------------------------------------------------------
 */
void RemainingCapacityAlarm_Write(uint16_t word);



/**
 * Sets or gets the Remaining Time alarm value. Whenever the AverageTimeToEmpty() falls below the
 * Remaining Time value, the Smart Battery sends AlarmWarning() messages to the SMBus Host with the
 * REMAINING_TIME_ALARM bit set. A Remaining Time value of 0 effectively disables this alarm.
 * (If the ALARM_MODE bit is set in BatteryMode() then the AlarmWarning() message is disabled for a set
 * period of time. See the BatteryMode() function for further information.)
 * The Remaining Time value is set to 10 minutes at time of manufacture. The Remaining Time value will
 * remain unchanged until altered by the RemainingTimeAlarm() function.
 * @details The RemainingTimeAlarm() function can be used by systems that want to adjust when the remaining time
 * alarm warning is sent. Since the time functions incorporate all aspects of the discharge (rate, temperature,
 * state-of-charge) they are better suited for predicting transition points to suspend or hibernate states. The
 * Remaining Time value can be read to verify the value in use by the Smart Battery's RemainingTimeAlarm().
 * SMBus Protocol: Read or Write Word
 * Address:         0x02
 * Input/Output:    unsigned int -- the point below which Remaining Time messages will be sent
 * Units:           minutes
 * Range:           0 to 65,535 minutes
 * Granularity:     not applicable
 * Accuracy:        see AverageTimeToEmpty()
 */
uint16_t RemainingTimeAlarm_Read(void);



/**
 * Sets or gets the Remaining Time alarm value. Whenever the AverageTimeToEmpty() falls below the
 * Remaining Time value, the Smart Battery sends AlarmWarning() messages to the SMBus Host with the
 * REMAINING_TIME_ALARM bit set. A Remaining Time value of 0 effectively disables this alarm.
 * (If the ALARM_MODE bit is set in BatteryMode() then the AlarmWarning() message is disabled for a set
 * period of time. See the BatteryMode() function for further information.)
 * The Remaining Time value is set to 10 minutes at time of manufacture. The Remaining Time value will
 * remain unchanged until altered by the RemainingTimeAlarm() function.
 * @details The RemainingTimeAlarm() function can be used by systems that want to adjust when the remaining time
 * alarm warning is sent. Since the time functions incorporate all aspects of the discharge (rate, temperature,
 * state-of-charge) they are better suited for predicting transition points to suspend or hibernate states. The
 * Remaining Time value can be read to verify the value in use by the Smart Battery's RemainingTimeAlarm().
 * SMBus Protocol: Read or Write Word
 * Address:         0x02
 * Input/Output:    unsigned int -- the point below which Remaining Time messages will be sent
 * Units:           minutes
 * Range:           0 to 65,535 minutes
 * Granularity:     not applicable
 * Accuracy:        see AverageTimeToEmpty()
 */
void RemainingTimeAlarm_Write(uint16_t word);



/**
 * This function selects the various battery operational modes and reports the battery’s capabilities, modes,
 * and flags minor conditions requiring attention.
 * Defined capabilities include:
 * · Internal charge controller supported (INTERNAL_CHARGE_CONTROLLER bit)
 * · Internal primary battery control supported (PRIMARY_BATTERY_SUPPORT bit)
 *   Note: These capabilities listed are optional but their indicating flag bits must be supported.
 * Defined modes include:
 * · Battery's capacity information is specified to be reported in either mAh or 10 mWh (CAPACITY_MODE bit)
 * · Whether the ChargingCurrent() and ChargingVoltage() values are to be broadcast to the Smart Battery
 *   Charger when the Smart Battery requires charging (CHARGER_MODE bit)
 * · Internal charge controller enable (CHARGE_CONTROLLER_ENABLED bit) [Optional]
 * · Internal primary battery control enable (PRIMARY_BATTERY bit) [Optional]
 * Defined conditions requiring attention include:
 * · Battery requesting a conditioning cycle (CONDITION_FLAG bit)
 * @details To allow configuration of the Smart Battery for particular application requirements. (See individual bit
 * definitions which follow.)
 * SMBus Protocol: Read or Write Word
 * Address:         0x03
 * Input/Output:    unsigned int - bit mapped - see below
 * Units:           not applicable
 * Range:           0...1
 * Granularity:     not applicable
 * Accuracy:        not applicable
 * See http://sbs-forum.org/specs/sbdat110.pdf page 14 for more details
 */
uint16_t BatteryMode_Read(void);



/**
 * This function selects the various battery operational modes and reports the battery’s capabilities, modes,
 * and flags minor conditions requiring attention.
 * Defined capabilities include:
 * · Internal charge controller supported (INTERNAL_CHARGE_CONTROLLER bit)
 * · Internal primary battery control supported (PRIMARY_BATTERY_SUPPORT bit)
 *   Note: These capabilities listed are optional but their indicating flag bits must be supported.
 * Defined modes include:
 * · Battery's capacity information is specified to be reported in either mAh or 10 mWh (CAPACITY_MODE bit)
 * · Whether the ChargingCurrent() and ChargingVoltage() values are to be broadcast to the Smart Battery
 *   Charger when the Smart Battery requires charging (CHARGER_MODE bit)
 * · Internal charge controller enable (CHARGE_CONTROLLER_ENABLED bit) [Optional]
 * · Internal primary battery control enable (PRIMARY_BATTERY bit) [Optional]
 * Defined conditions requiring attention include:
 * · Battery requesting a conditioning cycle (CONDITION_FLAG bit)
 * @details To allow configuration of the Smart Battery for particular application requirements. (See individual bit
 * definitions which follow.)
 * SMBus Protocol: Read or Write Word
 * Address:         0x03
 * Input/Output:    unsigned int - bit mapped - see below
 * Units:           not applicable
 * Range:           0...1
 * Granularity:     not applicable
 * Accuracy:        not applicable
 * See http://sbs-forum.org/specs/sbdat110.pdf page 14 for more details
 */
void BatteryMode_Write(uint16_t word);



/**
 * The AtRate() function is the first half of a two-function call-set used to set the AtRate value used in
 * calculations made by the AtRateTimeToFull(), AtRateTimeToEmpty(), and AtRateOK() functions. The
 * AtRate value may be expressed in either current (mA) or power (10mW) depending on the setting of the
 * BatteryMode()'s CAPACITY_MODE bit. (Configuration of the CAPACITY_MODE bit will alter the
 * calculation of AtRate functions. Changing the state of CAPACITY_MODE may require a re-write to the
 * AtRate() function using the appropriate units.)
 * @details Since the AtRate() function is the first half of a two-function call-set, it is followed by the second function
 * of the call-set that calculates and returns a value based on the AtRate value and the battery's present state:
 * · When the AtRate value is positive, the AtRateTimeToFull() function returns the predicted time to fullcharge at the AtRate value of charge. (This does NOT include the present charge or discharge rate and
 *   so is calculated independently from the present charge or discharge rate of the battery.)
 * · When the AtRate value is negative, the AtRateTimeToEmpty() function returns the predicted operating
 *   time at the AtRate value of discharge. (This does NOT include the present charge or discharge rate and
 *   so is calculated independently from the present charge or discharge rate of the battery.)
 * · When the AtRate value is negative, the AtRateOK() function returns a Boolean value that predicts the
 *   battery's ability to supply the AtRate value of additional discharge energy (current or power) for a
 *   minimum of 10 seconds. (This DOES include the present discharge rate of the battery and so is
 *   calculated differently from the previous ‘Time’ values listed.)
 * Timing Note: The Smart Battery may NACK the second function and return a ‘Busy’ error code in
 *              BatteryStatus() while calculating the result based on a new AtRate() value. Conversely, the Smart Battery
 *              may use clock stretching during the second function to allow time to calculate the result.
 * SMBus Protocol: Read or Write Word
 * Address:         0x04
 * Input/Output:
 * ---------------------------------------------------------------------------
 * |                             Battery Mode                                |
 * |-------------------------------------------------------------------------|
 * |                 |    CAPACITY_MODE bit = 0   |   CAPACITY_MODE bit = 1  |
 * |-------------------------------------------------------------------------|
 * |      units      |               mA           |           10mW           |
 * |-------------------------------------------------------------------------|
 * |   Charge Range: |      1 to 32,767 mA        |     1 to 32,767 10mW     |
 * |-------------------------------------------------------------------------|
 * |Discharge Range: |      1 to 32,767 mA        |     1 to 32,767 10mW     |
 * |-------------------------------------------------------------------------|
 * |   Granularity   |                         1 unit                        |
 * |-------------------------------------------------------------------------|
 * |     Accuracy    |                     not applicable                    |
 * ---------------------------------------------------------------------------
 * See http://sbs-forum.org/specs/sbdat110.pdf page 21 for more details
 */
uint16_t AtRate_Read(void);



/**
 * The AtRate() function is the first half of a two-function call-set used to set the AtRate value used in
 * calculations made by the AtRateTimeToFull(), AtRateTimeToEmpty(), and AtRateOK() functions. The
 * AtRate value may be expressed in either current (mA) or power (10mW) depending on the setting of the
 * BatteryMode()'s CAPACITY_MODE bit. (Configuration of the CAPACITY_MODE bit will alter the
 * calculation of AtRate functions. Changing the state of CAPACITY_MODE may require a re-write to the
 * AtRate() function using the appropriate units.)
 * @details Since the AtRate() function is the first half of a two-function call-set, it is followed by the second function
 * of the call-set that calculates and returns a value based on the AtRate value and the battery's present state:
 * · When the AtRate value is positive, the AtRateTimeToFull() function returns the predicted time to fullcharge at the AtRate value of charge. (This does NOT include the present charge or discharge rate and
 *   so is calculated independently from the present charge or discharge rate of the battery.)
 * · When the AtRate value is negative, the AtRateTimeToEmpty() function returns the predicted operating
 *   time at the AtRate value of discharge. (This does NOT include the present charge or discharge rate and
 *   so is calculated independently from the present charge or discharge rate of the battery.)
 * · When the AtRate value is negative, the AtRateOK() function returns a Boolean value that predicts the
 *   battery's ability to supply the AtRate value of additional discharge energy (current or power) for a
 *   minimum of 10 seconds. (This DOES include the present discharge rate of the battery and so is
 *   calculated differently from the previous ‘Time’ values listed.)
 * Timing Note: The Smart Battery may NACK the second function and return a ‘Busy’ error code in
 *              BatteryStatus() while calculating the result based on a new AtRate() value. Conversely, the Smart Battery
 *              may use clock stretching during the second function to allow time to calculate the result.
 * SMBus Protocol: Read or Write Word
 * Address:         0x04
 * Input/Output:
 * ---------------------------------------------------------------------------
 * |                             Battery Mode                                |
 * |-------------------------------------------------------------------------|
 * |                 |    CAPACITY_MODE bit = 0   |   CAPACITY_MODE bit = 1  |
 * |-------------------------------------------------------------------------|
 * |      units      |               mA           |           10mW           |
 * |-------------------------------------------------------------------------|
 * |   Charge Range: |      1 to 32,767 mA        |     1 to 32,767 10mW     |
 * |-------------------------------------------------------------------------|
 * |Discharge Range: |      1 to 32,767 mA        |     1 to 32,767 10mW     |
 * |-------------------------------------------------------------------------|
 * |   Granularity   |                         1 unit                        |
 * |-------------------------------------------------------------------------|
 * |     Accuracy    |                     not applicable                    |
 * ---------------------------------------------------------------------------
 * See http://sbs-forum.org/specs/sbdat110.pdf page 21 for more details
 */
void AtRate_Write(uint16_t word);



/**
 * Returns the predicted remaining time to fully charge the battery at the previously written AtRate value in mA.
 * Note: This function is only required to return a value when the CAPACITY_MODE bit is cleared and the
 * AtRate() value is written in mA units. If the CAPACITY_MODE bit is set, then AtRateTimeToFull() may
 * return 65535 to indicate over-range and return an error code indicating overflow. Alternately, this function
 * may return a remaining time to full based on a 10 mW value in AtRate().
 * All other AtRate functions are required to return both values corresponding to the CAPACITY_MODE
 * setting except AtRateTimeToFull(). Support for power capacity (10 mW) reporting in AtRateTimeToFull() is optional.
 * @details The AtRateTimeToFull() function is part of a two-function call-set used to determine the predicted
 * remaining charge time at the AtRate value (mA.) It will be used immediately after the SMBus Host sets the
 * AtRate() value. The calculated AtRateTimeToFull() value is independent of the present charge or
 * discharge rate of the battery. Refer to AtRate() for additional usage information.
 * SMBus Protocol: Read Word
 * Address:         0x05
 * Output:          unsigned int -- predicted time in minutes to fully charge the battery
 * Units:           minutes
 * Range:           0 to 65,534 min
 * Granularity:     2 min or better
 * Accuracy:        ±MaxError()*FullChargeCapacity() ¸ |AtRate()|
 * See http://sbs-forum.org/specs/sbdat110.pdf page 22 for more details
 */
uint16_t AtRateTimeToFull_Read(void);



/**
 * Returns the predicted remaining operating time if the battery is discharged at the previously written AtRate
 * value. (Result will depend on the setting of CAPACITY_MODE bit.)
 * @details The AtRateTimeToEmpty() function is part of a two-function call-set used to determine the remaining
 * operating time at the AtRate value. It will be used immediately after the SMBus Host sets the AtRate value.
 * The calculated AtRateTimeToEmpty() value is independent of the present charge or discharge rate of the
 * battery. Refer to AtRate() for additional usage information.
 * SMBus Protocol: Read Word
 * Address:         0x06
 * Output:          unsigned int -- estimated operating time left
 * Units:           minutes
 * Range:           0 to 65,534 min
 * Granularity:     2 min or better
 * Accuracy:        -0,+MaxError()*FullChargeCapacity() ¸ |AtRate()|
 * See http://sbs-forum.org/specs/sbdat110.pdf page 22 for more details
 */
uint16_t AtRateTimeToEmpty_Read(void);



/**
 * Returns a Boolean value that indicates whether or not the battery can deliver the previously written AtRate
 * value of additional energy for 10 seconds (Boolean). If the AtRate value is zero or positive, the
 * AtRateOK() function will ALWAYS return true. Result may depend on the setting of CAPACITY_MODE bit.
 * @details The AtRateOK() function is part of a two-function call-set used by power management systems to
 * determine if the battery can safely supply enough energy for an additional load. It will be used immediately
 * after the SMBus Host sets the AtRate value. Refer to AtRate() for additional usage information.
 * SMBus Protocol: Read Word
 * Address:         0x07
 * Output:          Boolean -- indicates if the battery can supply the additional energy requested
 * Units:           Boolean
 * Range:           TRUE (non-zero), FALSE (zero)
 * Granularity:     not applicable
 * Accuracy:        not applicable
 * See http://sbs-forum.org/specs/sbdat110.pdf page 23 for more details
 */
bool AtRateOK_Read(void);



/**
 * Returns the cell-pack's internal temperature (°K). The actual operational temperature range will be defined
 * at a pack level by a particular manufacturer. Typically it will be in the range of -20°C to +75°C.
 * @details The Temperature() function provides accurate cell temperatures for use by battery chargers and thermal
 * management systems. A battery charger will be able to use the temperature as a safety check. Thermal
 * management systems may use the temperature because the battery is one of the largest thermal sources in a
 * system. (Kelvin units are used to facilitate simple unsigned handling of temperature information and to
 * permit easy conversion to other units.)
 * SMBus Protocol: Read Word
 * Address:         0x08
 * Output:          unsigned int -- cell temperature in tenth degree Kelvin increments
 * Units:           0.1°K
 * Range:           0 to +6553.5°K
 * Granularity:     0.5°K or better
 * Accuracy:        ±3°K
 * See http://sbs-forum.org/specs/sbdat110.pdf page 23 for more details
 */
uint16_t Temperature_Read(void);



/**
 * Returns the cell-pack voltage (mV).
 * @details The Voltage() function provides power management systems with an accurate battery terminal voltage.
 * Power management systems can use this voltage, along with battery current information, to characterize
 * devices they control. This ability will help enable intelligent, adaptive power management systems.
 * SMBus Protocol: Read Word
 * Address:         0x09
 * Output:          unsigned int -- battery terminal voltage in milli-volts
 * Units:           mV
 * Range:           0 to 65,535 mV
 * Granularity:     0.2% of DesignVoltage()
 * Accuracy:        ±1.0% of DesignVoltage()
 * See http://sbs-forum.org/specs/sbdat110.pdf page 24 for more details
 */
uint16_t Voltage_Read(void);



/**
 * Returns the current being supplied (or accepted) through the battery's terminals (mA).
 * @details The Current() function provides a snapshot for the power management system of the current flowing into or
 * out of the battery. This information will be of particular use in power management systems because they
 * can characterize individual devices and "tune" their operation to actual system power behavior.
 * SMBus Protocol: Read Word
 * Address:         0x0A
 * Output:          signed int -- charge/discharge rate in mA increments - positive for charge, negative for discharge
 * Units:           mA
 * Range:           0 to 32,767 mA for charge or
 *                  0 to -32,768 mA for discharge
 * Granularity:     0.2% of the DesignCapacity() or better
 * Accuracy:        ±1.0% of the DesignCapacity()
 * See http://sbs-forum.org/specs/sbdat110.pdf page 24 for more details
 */
int16_t Current_Read(void);



/**
 * Returns a one-minute rolling average based on the current being supplied (or accepted) through the battery's
 * terminals (mA). The AverageCurrent() function is expected to return meaningful values during the battery's
 * first minute of operation.
 * @details The AverageCurrent() function provides the average current flowing into or out of the battery for the power
 * management system.
 * SMBus Protocol: Read Word
 * Address:         0x0B
 * Output:          signed int -- charge/discharge rate in mA increments - positive for charge, negative for discharge
 * Units:           mA
 * Range:           0 to 32,767 mA for charge or 0 to -32,768 mA for discharge
 * Granularity:     0.2% of the DesignCapacity() or better
 * Accuracy:        ±1.0% of the DesignCapacity()
 * See http://sbs-forum.org/specs/sbdat110.pdf page 24 for more details
 */
int32_t AverageCurrent_Read(void);



/**
 * Returns the expected margin of error (%) in the state of charge calculation. For example, when MaxError()
 * returns 10% and RelativeStateOfCharge() returns 50%, the Relative StateOfCharge() is actually between 50
 * and 60%. The MaxError() of a battery is expected to increase until the Smart Battery identifies a condition
 * that will give it higher confidence in its own accuracy. For example, when a Smart Battery senses that it has
 * been fully charged from a fully discharged state, it may use that information to reset or partially reset
 * MaxError(). The Smart Battery can signal when MaxError() has become too high by setting the
 * CONDITION_FLAG bit in BatteryMode().
 * @details The MaxError() function does not exist on most systems today. It has real value to the user in two ways:
 * first, to give the user a confidence level about the state of charge and second, to give the Power
 * Management system information about how aggressive it should be, particularly as the battery nears the end of its life.
 * SMBus Protocol: Read Word
 * Address:         0x0C
 * Output:          unsigned int -- percent uncertainty for selected information
 * Units:           %
 * Range:           0 to 100%
 * Granularity:     1%
 * Accuracy:        not applicable
 * See http://sbs-forum.org/specs/sbdat110.pdf page 25 for more details
 */
uint16_t MaxError_Read(void);



/**
 * Returns the predicted remaining battery capacity expressed as a percentage of FullChargeCapacity() (%).
 * @details The RelativeStateOfCharge() function exists on most systems today (a.k.a. Fuel Gauge). It is used to
 * estimate the amount of charge remaining in the battery. The problem with this paradigm is that the tank size
 * is variable. As standardized battery packs come into service, physical size will have less to do with the
 * actual capacity. Although the RelativeStateOfCharge() will continue to be used, new paradigms will be
 * developed to communicate battery capacity, thus diminishing it's importance.
 * SMBus Protocol: Read Word
 * Address:         0x0D
 * Output:          unsigned int -- percent of remaining capacity
 * Units:           %
 * Range:           0 to 100%
 * Granularity:     1%
 * Accuracy:        -0, +MaxError()
 * See http://sbs-forum.org/specs/sbdat110.pdf page 25 for more details
 */
uint16_t RelativeStateOfCharge_Read(void);



/**
 * Returns the predicted remaining battery capacity expressed as a percentage of DesignCapacity() (%). Note
 * that AbsoluteStateOfCharge() can return values greater than 100%.
 * @details See RelativeStateOfCharge() function description.
 * SMBus Protocol: Read Word
 * Address:         0x0E
 * Output:          unsigned int -- percent of remaining capacity
 * Units:           %
 * Range:           0 to 100%
 * Granularity:     1%
 * Accuracy:        -0, +MaxError()
 * See http://sbs-forum.org/specs/sbdat110.pdf page 25 for more details
 */
uint16_t AbsoluteStateOfCharge_Read(void);



/**
 * Returns the predicted remaining battery capacity. The RemainingCapacity() capacity value is expressed in
 * either current (mAh at a C/5 discharge rate) or power (10mWh at a P/5 discharge rate) depending on the
 * setting of the BatteryMode()'s CAPACITY_MODE bit.
 * @details The RemainingCapacity() function returns the battery's remaining capacity in absolute terms but relative to
 * a specific discharge rate. This information is a numeric indication of remaining charge which can also be
 * represented by the Absolute or Relative StateOfCharge() functions and may be in a better form for use by
 * power management systems. (StateOfCharge() functions return values in percentage format which is a
 * relative representation while RemainingCapacity() function returns a more absolute value defined at a
 * specific discharge value.)
 * SMBus Protocol: Read Word
 * Address:         0x0F
 * Output:          unsigned int -- remaining charge in mAh or 10mWh
 * ---------------------------------------------------------------------------
 * |                             Battery Mode                                |
 * |-------------------------------------------------------------------------|
 * |               |    CAPACITY_MODE bit = 0   |   CAPACITY_MODE bit = 1    |
 * |-------------------------------------------------------------------------|
 * |    units      |              mAh           |           10mWh            |
 * |-------------------------------------------------------------------------|
 * |    Range      |      0 to 65,535 mAh       |     0 to 65,535 10mWh      |
 * |-------------------------------------------------------------------------|
 * | Granularity   |          0.2% of DesignCapacity() or better             |
 * |-------------------------------------------------------------------------|
 * |    Accuracy   |        -0, +MaxError() * FullChargeCapacity()           |
 * ---------------------------------------------------------------------------
 * See http://sbs-forum.org/specs/sbdat110.pdf page 26 for more details
 */
uint16_t RemainingCapacity_Read(void);



/**
 * Returns the predicted pack capacity when it is fully charged. The FullChargeCapacity() value is expressed
 * in either current (mAh at a C/5 discharge rate) or power (10mWh at a P/5 discharge rate) depending on the
 * setting of the BatteryMode()'s CAPACITY_MODE bit.
 * @details The FullChargeCapacity() function provides the user with a means of understanding the "tank size" of their
 * battery. This information, along with information about the original capacity of the battery, can be
 * presented to the user as an indication of battery wear.
 * SMBus Protocol: Read Word
 * Address:         0x10
 * Output:          unsigned int -- estimated full charge capacity in mAh or 10mWh
 * ---------------------------------------------------------------------------
 * |                             Battery Mode                                |
 * |-------------------------------------------------------------------------|
 * |               |    CAPACITY_MODE bit = 0   |   CAPACITY_MODE bit = 1    |
 * |-------------------------------------------------------------------------|
 * |    units      |              mAh           |           10mWh            |
 * |-------------------------------------------------------------------------|
 * |    Range      |      0 to 65,535 mAh       |     0 to 65,535 10mWh      |
 * |-------------------------------------------------------------------------|
 * | Granularity   |          0.2% of DesignCapacity() or better             |
 * |-------------------------------------------------------------------------|
 * |    Accuracy   |        -0, +MaxError() * FullChargeCapacity()           |
 * --------------------------------------------------------------------------|
 * See http://sbs-forum.org/specs/sbdat110.pdf page 26 for more details
 */
uint16_t FullChargeCapacity_Read(void);



/**
 * Returns the predicted remaining battery life at the present rate of discharge (minutes). The
 * RunTimeToEmpty() value is calculated based on either current or power depending on the setting of the
 * BatteryMode()'s CAPACITY_MODE bit. This is an important distinction because use of the wrong
 * calculation mode may result in inaccurate return values.
 * @details The RunTimeToEmpty() can be used by the power management system to get information about the relative
 * gain or loss in remaining battery life in response to a change in power policy. This information is NOT the
 * same as the AverageTimeToEmpty(), which is not suitable to determine the effects that result from a change
 * in power policy.
 * SMBus Protocol: Read Word
 * Address:         0x11
 * Output:          unsigned int -- minutes of operation left
 * Units:           minutes
 * Range:           0 to 65,534 min
 * Granularity:     2 min or better
 * Accuracy:        -0, +MaxError() * FullChargeCapacity() / Current()
 * Invalid Data Indication: 65,535 indicates battery is not being discharged
 * See http://sbs-forum.org/specs/sbdat110.pdf page 27 for more details
 */
uint16_t RunTimeToEmpty_Read(void);



/**
 * Returns a one-minute rolling average of the predicted remaining battery life (minutes). The
 * AverageTimeToEmpty() value is calculated based on either current or power depending on the setting of
 * the BatteryMode()'s CAPACITY_MODE bit. This is an important distinction because use of the wrong
 * calculation mode may result in inaccurate return values.
 * @details The AverageTimeToEmpty() displays state-of-charge information in a more useful way. By averaging the
 * instantaneous estimations, the remaining time will not appear to "jump" around as it does on many of today's systems.
 * SMBus Protocol: Read Word
 * Address:         0x12
 * Output:          unsigned int -- minutes of operation left
 * Units:           minutes
 * Range:           0 to 65,534 min
 * Granularity:     2 min or better
 * Accuracy:        -0, +MaxError() * FullChargeCapacity() / AverageCurrent()
 * Invalid Data Indication: 65,535 indicates battery is not being discharged
 * See http://sbs-forum.org/specs/sbdat110.pdf page 27 for more details
 */
uint16_t AverageTimeToEmpty_Read(void);



/**
 * Returns a one minute rolling average of the predicted remaining time until the Smart Battery reaches full
 * charge (minutes).
 * @details The AverageTimeToFull() function can be used by the SMBus Host's power management system to aid in
 * its policy. It may also be used to find out how long the system must be left on to achieve full charge.
 * SMBus Protocol: Read Word
 * Address:         0x13
 * Output:          unsigned int -- remaining time in minutes
 * Units:           minutes
 * Range:           0 to 65,534 min
 * Granularity:     2 min or better
 * Accuracy:        ±MaxError() * FullChargeCapacity() / AverageCurrent()
 * Invalid Data Indication: 65,535 indicates the battery is not being charged
 * See http://sbs-forum.org/specs/sbdat110.pdf page 27 for more details
 */
uint16_t AverageTimeToFull_Read(void);



/**
 * Returns a one minute rolling average of the predicted remaining time until the Smart Battery reaches full
 * charge (minutes).
 * @details The AverageTimeToFull() function can be used by the SMBus Host's power management system to aid in
 * its policy. It may also be used to find out how long the system must be left on to achieve full charge.
 * SMBus Protocol: Read Word
 * Address:         0x16
 * Output:          unsigned int - Status Register with alarm conditions bit mapped as follows:
 * * * * * * * Alarm Bits * * * * *
 * 0x8000 OVER_CHARGED_ALARM
 * 0x4000 TERMINATE_CHARGE_ALARM
 * 0x2000 Reserved
 * 0x1000 OVER_TEMP_ALARM
 * 0x0800 TERMINATE_DISCHARGE_ALARM
 * 0x0400 Reserved
 * 0x0200 REMAINING_CAPACITY_ALARM
 * 0x0100 REMAINING_TIME_ALARM
 * * * * * * * Status Bits * * * * *
 * 0x0080 INITIALIZED
 * 0x0040 DISCHARGING
 * 0x0020 FULLY_CHARGED
 * 0x0010 FULLY_DISCHARGED
 * * * * * * * Error Code * * * * *
 * 0x0000-0x000f Reserved for error codes - See Appendix C
 * See http://sbs-forum.org/specs/sbdat110.pdf page 28,29,30,31 for more details
 */
uint16_t BatteryStatus_Read(void);



/**
 * Returns the number of cycles the battery has experienced. A cycle is defined as:
 * An amount of discharge approximately equal to the value of DesignCapacity.
 * @details The CycleCount() function provides a means to determine their battery's wear. It may be used to give
 * advanced warning that the battery is nearing its end of life. The CycleCount returned value multiplied by
 * the DesignCapacity value can give an approximate “odometer” reading for the total capacity delivered by
 * the Smart Battery.
 * SMBus Protocol: Read Word
 * Address:         0x17
 * Output:          unsigned int -- count of charge/discharge cycles the battery has experienced
 * Units:           cycle
 * Range:           0 to 65,534 cycles
 *                  65,535 indicates battery has experienced 65,535 or more cycles.
 * Granularity:     1 cycle
 * Accuracy:        absolute count
 * See http://sbs-forum.org/specs/sbdat110.pdf page 32 for more details
 */
uint16_t CycleCount_Read(void);



/**
 * Returns the theoretical capacity of a new pack. The DesignCapacity() value is expressed in either current
 * (mAh at a C/5 discharge rate) or power (10mWh at a P/5 discharge rate) depending on the setting of the
 * BatteryMode()'s CAPACITY_MODE bit.
 * @details The DesignCapacity() function is used by the SMBus Host's power management in conjunction with
 * FullChargeCapacity() to determine battery wear. The power management system may present this
 * information to the user and also adjust its power policy as a result.
 * SMBus Protocol: Read Word
 * Address:         0x18
 * @return          unsigned int -- battery capacity in mAh or 10mWh
 * ---------------------------------------------------------------------------
 * |                             Battery Mode                                |
 * |-------------------------------------------------------------------------|
 * |               |    CAPACITY_MODE bit = 0   |   CAPACITY_MODE bit = 1    |
 * |-------------------------------------------------------------------------|
 * |    units      |             mAh            |            10mWh           |
 * |-------------------------------------------------------------------------|
 * |    Range      |      0 to 65,535 mAh       |     0 to 65,535 10mWh      |
 * |-------------------------------------------------------------------------|
 * | Granularity   |                     not applicable                      |
 * |-------------------------------------------------------------------------|
 * |    Accuracy   |                     not applicable                      |
 * --------------------------------------------------------------------------|
 * See http://sbs-forum.org/specs/sbdat110.pdf page 32 for more details
 */
uint16_t DesignCapacity_Read(void);



/**
 * Returns the theoretical voltage of a new pack (mV).
 * @details The DesignVoltage() function can be used to give additional information about a particular Smart Battery's
 * expected terminal voltage
 * SMBus Protocol: Read Word
 * Address:         0x19
 * Output:          unsigned int -- the battery's designed terminal voltage in mV
 * Units:           mV
 * Range:           0 to 65,535 mV
 * Granularity:     not applicable
 * Accuracy:        not applicable
 * See http://sbs-forum.org/specs/sbdat110.pdf page 32 for more details
 */
uint16_t DesignVoltage_Read(void);



/**
 * Returns the version number of the Smart Battery specification the battery pack supports, as well as voltage
 * and current and capacity scaling information in a packed unsigned integer.
 * @details The SpecificationInfo() function is used by the SMBus Host's power management system to determine what
 * information the Smart Battery can provide. It can be used by Smart Battery Systems where the defined 16-
 * bit data values do not provide enough range for higher power applications.
 * SMBus Protocol: Read Word
 * Address:         0x1A
 * Output:          unsigned int -- packed specification number and scaling information
 * See http://sbs-forum.org/specs/sbdat110.pdf page 33 for more details
 */
uint16_t SpecificationInfo_Read(void);



/**
 * This function returns the date the cell pack was manufactured in a packed integer. The date is packed in the
 * following fashion: (year-1980) * 512 + month * 32 + day.
 * Due to the 1980 offset, there is no ‘Year 2000’ issue with the encoding of this function.
 * @details The ManufactureDate() provides the system with information that can be used to uniquely identify a
 * particular battery.
 * SMBus Protocol: Read Word
 * Address:         0x1B
 * Output:          unsigned int -- packed date of manufacture
 * See http://sbs-forum.org/specs/sbdat110.pdf page 34 for more details
 */
SMB_tDate ManufactureDate_Read(void);



/**
 * This function is used to return a serial number. This number when combined with the ManufacturerName(),
 * the DeviceName(), and the ManufactureDate() will uniquely identify the battery (unsigned int).
 * @details The SerialNumber() function is used to identify a particular battery. This may be important in systems that
 * are powered by multiple batteries where the system can log information about each battery that it encounters.
 * SMBus Protocol: Read Word
 * Address:         0x1C
 * Output:          unsigned int
 * See http://sbs-forum.org/specs/sbdat110.pdf page 34 for more details
 */
uint16_t SerialNumber_Read(void);



/**
 * This function returns a character array containing the battery's manufacturer's name. For example,
 * "MyBattCo" would identify the Smart Battery's manufacturer as MyBattCo.
 * @details The ManufacturerName() function returns the name of the Smart Battery's manufacturer. The
 * manufacturer's name can be displayed by the SMBus Host's power management system display as both an
 * identifier and as an advertisement for the manufacturer. The name is also useful as part of the information
 * required to uniquely identify a battery.
 * SMBus Protocol: Read Block
 * Address:         0x20
 * Output:          string -- character string
 * See http://sbs-forum.org/specs/sbdat110.pdf page 34 for more details
 */
void ManufacturerName_Read(uint8_t* Data);



/**
 * This function returns a character string that contains the battery's name. For example, a DeviceName() of
 * "MBC101" would indicate that the battery is a model MBC101.
 * @details The DeviceName() function returns the battery's name for display by the SMBus Host's power management
 * system as well as for identification purposes.
 * SMBus Protocol: Read Block
 * Address:         0x21
 * Output:          string -- character string
 * See http://sbs-forum.org/specs/sbdat110.pdf page 34 for more details
 */
void DeviceName_Read(uint8_t* Data);



/**
 * This function returns a character string that contains the battery's chemistry. For example, if the
 * DeviceChemistry() function returns "NiMH," the battery pack would contain nickel metal hydride cells.
 * @details The DeviceChemistry() function gives cell chemistry information for use by charging systems.
 * SMBus Protocol: Read Block
 * Address:         0x22
 * Output:          string -- character string
 * See http://sbs-forum.org/specs/sbdat110.pdf page 35 for more details
 */
void DeviceChemistry_Read(uint8_t* Data);



/**
 * This function allows access to the manufacturer data contained in the battery (data).
 * @details The ManufacturerData() function may be used to access the manufacturer's data area. The information and
 * its format are proprietary, but might include items such as: lot codes, number of deep cycles, discharge
 * patterns, deepest discharge, etc. The Smart Battery manufacturer is free to use this data as they see fit.
 * SMBus Protocol: Read Block
 * Address:         0x22
 * Output:          block data - data whose meaning is assigned by the Smart Battery's manufacturer
 * See http://sbs-forum.org/specs/sbdat110.pdf page 35 for more details
 */
void ManufacturerData_ReadBlock(uint8_t Data);



void ToggleMosfetCMD();

#endif /* BMS_H_ */
