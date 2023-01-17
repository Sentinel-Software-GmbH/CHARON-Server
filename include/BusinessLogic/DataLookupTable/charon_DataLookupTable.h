/**
 *  Sentinel Software GmbH
 *  Copyright (C) 2022 Florian Schöner
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
/**
 * @addtogroup CharonUDS_Server
 * @{
 * @defgroup BusinessLogic Business Logic Modules
 * @{
 * @file charon_DataLookupTable.h
 * This Module handles the Data Management of the stored DataIdentifier for the system.
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

#ifndef CHARON_DATALOOKUPTABLE_H_
#define CHARON_DATALOOKUPTABLE_H_

/* Includes ******************************************************************/
#include <stdint.h>
#include <stdbool.h>

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/** This variable is for the maximum amount of data Length, this is needed for the initialization of the memory */
#define MAX_VARIABLE_DATA_LENGTH    ((uint32_t) 20u)

/**
 * @brief 
 * The Enumeration of the Saved Data Identifier
 */
typedef enum uds_data_t_public
{
    uds_data_test_did  = 0x4269,        /**< Test DataIdentifier for Debugging */
    uds_data_test_did2 = 0x6942,        /**< Test DataIdentifier for Debugging */
    uds_data_test_periodic = 0xF269,    /**< Test DataIdentifier for Debugging */
    uds_data_test_periodic1 = 0xF242,   /**< Test DataIdentifier for Debugging */
    uds_data_test_dynamic = 0xF301,     /**< Test DataIdentifier for Debugging */

    uds_data_amount
} uds_data_t;

/**
 * @brief 
 * The Charon Data IDentifier Object struct contains the Information for the Given Data Identifier.
 */
typedef struct charon_dataIdentifierObject_t_public
{
    uds_data_t      DID;                    /**< The Data Identifier  */
    uint32_t        sessionMask;            /**< stores the information in witch session the DID is allowed to be read  */
    uint32_t        lengthOfData;           /**< length of the stored Data in bytes */
    uint32_t        addressOfData;          /**< the address of the stored Data */
    bool            hasScalingData;         /**< true when the DID has scaling Data */
    uint32_t        lengthOfScalingData;    /**< length of the scaling Data in Bytes */
    uint32_t        addressOfScalingData;   /**< Address of the scaling data */
    bool            readOnlyDID;            /**< If true the information is Only to read for the client */
    bool            isDynamicallyDID;       /**< Dynamical defined DID*/

} charon_dataIdentifierObject_t;

/**
 * @brief 
 * A enumeration of the Scaling byte High Nibble from the ISO 14229-1
 */
typedef enum scalingByteHighNibble_t_public
{
    unSignedNumeric                 = 0x0,  /**< The Scaling Data is a Unsigned Numeric Value */  
    signedNumeric                   = 0x1,  /**< The Scaling Data is a Signed Numeric Value */
    bitMappedReportedWithOutMask    = 0x2,  /**< Bit mapped encoding uses individual bits or small groups of bits to represent status */
    bitMappedReportedWithMask       = 0x3,  /**< Bit mapped encoding uses individual bits or small groups of bits to represent status */
    binaryCodedDecimal              = 0x4,  /**< Conventional Binary Coded Decimal encoding is used to represent two numeric digits per byte */
    stateEncodedVariable            = 0x5,  /**< This encoding uses a binary weighting scheme to represent up to 256 distinct states. */
    ASCII                           = 0x6,  /**< Conventional ASCII encoding */
    signedFloatingPoint             = 0x7,  /**< The Scaling Data is a Signed Floating Point Numeric Value */
    packet                          = 0x8,  /**< Packets contain multiple data values, usually related, each with unique scaling. */
    formula                         = 0x9,  /**< Scaling Data Contains a Formula */
    unit_format                     = 0xA,  /**< The units and formats are used to present the data in a more user-friendly format.  */
    stateAndConnectionType          = 0xB,  /**< This encoding is used especially for input and output signals.  */

    scalingByteHighNibble_amount
}scalingByteHighNibble_t;

/**
 * @brief 
 * This enum contains the Formula´s from the ISO 14229-1. 
 * As an addition a Manufacturer can add their own formula´s too.
 */
typedef enum formulaIdentifier_encoding_t_public
{
    formula0        = 0x00,     /**< y = C0 * x + C1 */
    formula1        = 0x01,     /**< y = C0 * (x + C1) */
    formula2        = 0x02,     /**< y = C0 / (x + C1) + C2 */
    formula3        = 0x03,     /**< y = x / C0 + C1 */
    formula4        = 0x04,     /**< y = (x + C0) / C1 */
    formula5        = 0x05,     /**< y = (x + C0) / C1 + C2 */
    formula6        = 0x06,     /**< y = C0 * x */
    formula7        = 0x07,     /**< y = x / C0 */
    formula8        = 0x08,     /**< y = x + C0 */
    formula9        = 0x09,     /**< y = x * C0 / C1 */
    VMS_formula10   = 0x80,     /**< place holder for the moment */
    VMS_formula11   = 0x81,     /**< place holder for the moment */

    formulaIdentifier_encoding_amount
}formulaIdentifier_encoding_t;

/**
 * @brief 
 * This enumeration contains the Unit´s for the Scaling Data.
 */
typedef enum Unit_Format_scaling_byte_t_public
{
    NoUnit_NoPrefix                 = 0x00,     
    Meter                           = 0x01,     /**< "m" Length */
    Foot                            = 0x02,     /**< "ft" Length */
    Inch                            = 0x03,     /**< "in" Length */
    Yard                            = 0x04,     /**< "yd" Length */
    mile                            = 0x05,     /**< "mi" length */
    Gram                            = 0x06,     /**< "g" mass */
    ton                             = 0x07,     /**< "t" mass */
    Second                          = 0x08,     /**< "s" time */
    Minute                          = 0x09,     /**< "min" time */
    Hour                            = 0x0A,     /**< "h" time */
    Day                             = 0x0B,     /**< "d" time */
    year                            = 0x0C,     /**< "y" time */
    ampere                          = 0x0D,     /**< "A" current */
    volt                            = 0x0E,     /**< "V" voltage */
    coulomb                         = 0x0F,     /**< "C" electric charge */
    ohm                             = 0x10,     /**< "W" resistance */
    farad                           = 0x11,     /**< "F" capacitance */
    henry                           = 0x12,     /**< "H" inductance */
    siemens                         = 0x13,     /**< "S" electric conductance */
    weber                           = 0x14,     /**< "Wb" magnetic flux */
    tesla                           = 0x15,     /**< "T" magnetic flux density */
    kelvin                          = 0x16,     /**< "K" thermodynamic temperature */
    Celsius                         = 0x17,     /**< "°C" thermodynamic temperature */
    Fahrenheit                      = 0x18,     /**< "°F" thermodynamic temperature */
    candela                         = 0x19,     /**< "cd" luminous intensity */
    radian                          = 0x1A,     /**< "rad" plane angle */
    degree                          = 0x1B,     /**< "°" plane angle */
    hertz                           = 0x1C,     /**< "Hz" frequency */
    joule                           = 0x1D,     /**< "J" energy */
    Newton                          = 0x1E,     /**< "N" force */
    kilopond                        = 0x1F,     /**< "kp" force */
    pound_force                     = 0x20,     /**< "lbf" force */
    watt                            = 0x21,     /**< "W" power */
    horse_power_metric              = 0x22,     /**< "hk" power */
    horse_power_UK_US               = 0x23,     /**< "hp" power */
    Pascal                          = 0x24,     /**< "Pa" pressure */
    bar                             = 0x25,     /**< "bar" pressure */
    atmosphere                      = 0x26,     /**< "atm" pressure */
    pound_force_per_square_inch     = 0x27,     /**< "psi" pressure */
    becqerel                        = 0x28,     /**< "Bq" radioactivity */
    lumen                           = 0x29,     /**< "lm" light flux */
    lux                             = 0x2A,     /**< "lx" illuminance */
    liter                           = 0x2B,     /**< "l" volume */
    gallon_british                  = 0x2C,     /**< volume */
    gallon_US_liq                   = 0x2D,     /**< volume */
    cubic_inch                      = 0x2E,     /**< "cu in" volume */
    meter_per_second                = 0x2F,     /**< "m/s" speed */
    kilometer_per_hour              = 0x30,     /**< "km/h" speed */
    mile_per_hour                   = 0x31,     /**< "mph" speed */
    revolutions_per_second          = 0x32,     /**< "rps" angular velocity */
    revolutions_per_minute          = 0x33,     /**< "rpm" angular velocity */
    counts                          = 0x34,
    percent                         = 0x35,     /**< "%" */
    milligram_per_stroke            = 0x36,     /**< "mg/stroke" mass per engine stroke */
    meter_per_square_second         = 0x37,     /**< "m/sP²P" acceleration */
    Newton_meter                    = 0x38,     /**< "Nm" moment */
    liter_per_minute                = 0x39,     /**< "l/min" flow */
    Watt_per_square_meter           = 0x3A,     /**< "W/mP²" Intensity */
    Bar_per_second                  = 0x3B,     /**< "bar/s" Pressure change */
    Radians_per_second              = 0x3C,     /**< "rad/s" Angular velocity */
    Radians_per_square_meter        = 0x3D,     /**< "rad/sP²P" Angular velocity */
    Kilogram_per_square_meter       = 0x3E,     /**< "kg/mP²P" */
    Reserved_by_document            = 0x3F,     /**< Reserved by document */
    exa_prefix                      = 0x40,     /**< "E" 1018 */
    peta_prefix                     = 0x41,     /**< "P" 1015 */
    tera_prefix                     = 0x42,     /**< "T" 1012 */
    giga_prefix                     = 0x43,     /**< "G" 109 */
    mega_prefix                     = 0x44,     /**< "M" 106 */
    kilo_prefix                     = 0x45,     /**< "k" 103 */
    hecto_prefix                    = 0x46,     /**< "h" 102 */
    deca_prefix                     = 0x47,     /**< "da" 10 */
    deci_prefix                     = 0x48,     /**< "d" 10-1 */
    centi_prefix                    = 0x49,     /**< "c" 10-2 */
    milli_prefix                    = 0x4A,     /**< "m" 10-3 */
    micro_prefix                    = 0x4B,     /**< "m" 10-6 */
    nano_prefix                     = 0x4C,     /**< "n" 10-9 */
    pico_prefix                     = 0x4D,     /**< "p" 10-12 */
    femto_prefix                    = 0x4E,     /**< "f" 10-15 */
    atto_prefix                     = 0x4F,     /**< "a" 10-18 */
    Date_1                          = 0x50,     /**< Year-Month-Day */
    Date_2                          = 0x51,     /**< Day/Month/Year */
    Date_3                          = 0x52,     /**< Month/Day/Year */
    week                            = 0x53,     /**< "W" calendar week */
    Time_1                          = 0x54,     /**< UTC Hour/Minute/Second */
    Time_2                          = 0x55,     /**< Hour/Minute/Second */
    DateAndTime_1                   = 0x56,     /**< Second/Minute/Hour/Day/Month/Year */
    DateAndTime_2                   = 0x57,     /**< Second/Minute/Hour/Day/Month/Year/Local minute offset/Local hour offset */
    DateAndTime_3                   = 0x58,     /**< Second/Minute/Hour/Month/Day/Year */
    DateAndTime_4                   = 0x59,     /**< Second/Minute/Hour/Month/Day/Year/Local minute offset/Local hour offset */

    Unit_Format_amount
}Unit_Format_scaling_byte_t;
/* Interfaces ****************************************************************/

/**
 * @brief 
 * this function initialises the lookup table with the write addresses based on the length of the Data of the previous entry.
 * 
 */
void charonDIDLookUpInit (void);

/**
 * @brief 
 * This function allows to get access of the data lookup table via the "DataIdentifier"
 * 
 * @param DID a two byte value that has to match a defined Data Identifier
 * @return charon_dataIdentifierObject_t* returns a pointer to the "dataArray" in the position of the given DataIdentifier
 */
charon_dataIdentifierObject_t* charon_getDataLookupTableByDID (uint16_t DID);

/**
 * @brief 
 * This function allows to get access of the data lookup table via the Address
 * 
 * @param DataAddress the address of a stored DataIdentifier
 * @return charon_dataIdentifierObject_t* returns a pointer to the "dataArray" in the position of the given Address
 */
charon_dataIdentifierObject_t* charon_getDataLookupTableByAddress (uint32_t DataAddress);

/** @todo add function to write data to DID and or address for the end user */

#endif /* CHARON_DATALOOKUPTABLE_H_ */

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
