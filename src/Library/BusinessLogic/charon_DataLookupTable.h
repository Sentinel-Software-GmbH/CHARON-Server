/**
 *  Sentinel Software GmbH
 *  Copyright (C) 2022 Andreas Hofmann
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
 * @addtogroup CharonUDS
 * @{
 * @defgroup BusinessLogic Business Logic
 * @{
 * @file charon_DataLookupTable.h
 * This Module handles the Receive and Transfer of the charon uds stack data.
 * It Controls encryption and checks for all demanded Timeouts.
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

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

typedef struct charon_dataIdentifierObject_t_public
{
    uds_data_t      DID; /* dataIdentifier */
    uint32_t        sessionMask;
    uint32_t        lengthOfData;
    uint32_t        AddressOfData;
    scalingByteHighNibble_t ScalingByteHighNibble;
    uint8_t ScalingByteLowNibble;

} charon_dataIdentifierObject_t;

typedef enum uds_data_t_public
{
    uds_data_test_did  = 0x4269,
    uds_data_test_did2 = 0x6942,

    uds_data_amount
} uds_data_t;

typedef enum scalingByteHighNibble_t_public
{
    unSignedNumeric                 = 0x0,
    signedNumeric                   = 0x1,
    bitMappedReportedWithOutMask    = 0x2,
    bitMappedReportedWithMask       = 0x3,
    binaryCodedDecimal              = 0x4,
    stateEncodedVariable            = 0x5,
    ASCII                           = 0x6,
    signedFloatingPoint             = 0x7,
    packet                          = 0x8,
    formula                         = 0x9,
    unit_format                     = 0xA,
    stateAndConnectionType          = 0xB,

    scalingByteHighNibble_amount
}scalingByteHighNibble_t;


typedef enum formulaIdentifier_encoding_t_public
{
    formula0        = 0x00,     /* y = C0 * x + C1 */
    formula1        = 0x01,     /* y = C0 * (x + C1) */
    formula2        = 0x02,     /* y = C0 / (x + C1) + C2 */
    formula3        = 0x03,     /* y = x / C0 + C1 */
    formula4        = 0x04,     /* y = (x + C0) / C1 */
    formula5        = 0x05,     /* y = (x + C0) / C1 + C2 */
    formula6        = 0x06,     /* y = C0 * x */
    formula7        = 0x07,     /* y = x / C0 */
    formula8        = 0x08,     /* y = x + C0 */
    formula9        = 0x09,     /* y = x * C0 / C1 */
    VMS_formula10   = 0x80,     /* place holder for the moment */
    VMS_formula11   = 0x81,     /* place holder for the moment */

    formulaIdentifier_encoding_amount
}formulaIdentifier_encoding_t;

typedef enum Unit_Format_scaling_byte_t_public
{
    NoUnit_NoPrefix                 = 0x00,     
    Meter                           = 0x01,     /* "m" Length */
    Foot                            = 0x02,     /* "ft" Length */
    Inch                            = 0x03,     /* "in" Length */
    Yard                            = 0x04,     /* "yd" Length */
    mile                            = 0x05,     /* "mi" length */
    Gram                            = 0x06,     /* "g" mass */
    ton                             = 0x07,     /* "t" mass */
    Second                          = 0x08,     /* "s" time */
    Minute                          = 0x09,     /* "min" time */
    Hour                            = 0x0A,     /* "h" time */
    Day                             = 0x0B,     /* "d" time */
    year                            = 0x0C,     /* "y" time */
    ampere                          = 0x0D,     /* "A" current */
    volt                            = 0x0E,     /* "V" voltage */
    coulomb                         = 0x0F,     /* "C" electric charge */
    ohm                             = 0x10,     /* "W" resistance */
    farad                           = 0x11,     /* "F" capacitance */
    henry                           = 0x12,     /* "H" inductance */
    siemens                         = 0x13,     /* "S" electric conductance */
    weber                           = 0x14,     /* "Wb" magnetic flux */
    tesla                           = 0x15,     /* "T" magnetic flux density */
    kelvin                          = 0x16,     /* "K" thermodynamic temperature */
    Celsius                         = 0x17,     /* "°C" thermodynamic temperature */
    Fahrenheit                      = 0x18,     /* "°F" thermodynamic temperature */
    candela                         = 0x19,     /* "cd" luminous intensity */
    radian                          = 0x1A,     /* "rad" plane angle */
    degree                          = 0x1B,     /* "°" plane angle */
    hertz                           = 0x1C,     /* "Hz" frequency */
    joule                           = 0x1D,     /* "J" energy */
    Newton                          = 0x1E,     /* "N" force */
    kilopond                        = 0x1F,     /* "kp" force */
    pound_force                     = 0x20,     /* "lbf" force */
    watt                            = 0x21,     /* "W" power */
    horse_power_metric              = 0x22,     /* "hk" power */
    horse_power_UK_US               = 0x23,     /* "hp" power */
    Pascal                          = 0x24,     /* "Pa" pressure */
    bar                             = 0x25,     /* "bar" pressure */
    atmosphere                      = 0x26,     /* "atm" pressure */
    pound_force_per_square_inch     = 0x27,     /* "psi" pressure */
    becqerel                        = 0x28,     /* "Bq" radioactivity */
    lumen                           = 0x29,     /* "lm" light flux */
    lux                             = 0x2A,     /* "lx" illuminance */
    liter                           = 0x2B,     /* "l" volume */
    gallon_british                  = 0x2C,     /* volume */
    gallon_US_liq                   = 0x2D,     /* volume */
    cubic_inch                      = 0x2E,     /* "cu in" volume */
    meter_per_second                = 0x2F,     /* "m/s" speed */
    kilometer_per_hour              = 0x30,     /* "km/h" speed */
    mile_per_hour                   = 0x31,     /* "mph" speed */
    revolutions_per_second          = 0x32,     /* "rps" angular velocity */
    revolutions_per_minute          = 0x33,     /* "rpm" angular velocity */
    counts                          = 0x34,
    percent                         = 0x35,     /* "%" */
    milligram_per_stroke            = 0x36,     /* "mg/stroke" mass per engine stroke */
    meter_per_square_second         = 0x37,     /* "m/sP²P" acceleration */
    Newton_meter                    = 0x38,     /* "Nm" moment */
    liter_per_minute                = 0x39,     /* "l/min" flow */
    Watt_per_square_meter           = 0x3A,     /* "W/mP²" Intensity */
    Bar_per_second                  = 0x3B,     /* "bar/s" Pressure change */
    Radians_per_second              = 0x3C,     /* "rad/s" Angular velocity */
    Radians_per_square_meter        = 0x3D,     /* "rad/sP²P" Angular velocity */
    Kilogram_per_square_meter       = 0x3E,     /* "kg/mP²P" */
    Reserved_by_document            = 0x3F,     /* Reserved by document */
    exa_prefix                      = 0x40,     /* "E" 1018 */
    peta_prefix                     = 0x41,     /* "P" 1015 */
    tera_prefix                     = 0x42,     /* "T" 1012 */
    giga_prefix                     = 0x43,     /* "G" 109 */
    mega_prefix                     = 0x44,     /* "M" 106 */
    kilo_prefix                     = 0x45,     /* "k" 103 */
    hecto_prefix                    = 0x46,     /* "h" 102 */
    deca_prefix                     = 0x47,     /* "da" 10 */
    deci_prefix                     = 0x48,     /* "d" 10-1 */
    centi_prefix                    = 0x49,     /* "c" 10-2 */
    milli_prefix                    = 0x4A,     /* "m" 10-3 */
    micro_prefix                    = 0x4B,     /* "m" 10-6 */
    nano_prefix                     = 0x4C,     /* "n" 10-9 */
    pico_prefix                     = 0x4D,     /* "p" 10-12 */
    femto_prefix                    = 0x4E,     /* "f" 10-15 */
    atto_prefix                     = 0x4F,     /* "a" 10-18 */
    Date_1                          = 0x50,     /* Year-Month-Day */
    Date_2                          = 0x51,     /* Day/Month/Year */
    Date_3                          = 0x52,     /* Month/Day/Year */
    week                            = 0x53,     /* "W" calendar week */
    Time_1                          = 0x54,     /* UTC Hour/Minute/Second */
    Time_2                          = 0x55,     /* Hour/Minute/Second */
    DateAndTime_1                   = 0x56,     /* Second/Minute/Hour/Day/Month/Year */
    DateAndTime_2                   = 0x57,     /* Second/Minute/Hour/Day/Month/Year/Local minute offset/Local hour offset */
    DateAndTime_3                   = 0x58,     /* Second/Minute/Hour/Month/Day/Year */
    DateAndTime_4                   = 0x59,     /* Second/Minute/Hour/Month/Day/Year/Local minute offset/Local hour offset */

    Unit_Format_amount
}Unit_Format_scaling_byte_t;
/* Interfaces ****************************************************************/

charon_dataIdentifierObject_t* charon_getDataLookupTable (uint16_t DID, uint32_t DataAddress);

#endif /* CHARON_DATALOOKUPTABLE_H_ */

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
