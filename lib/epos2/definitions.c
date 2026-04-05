// *******************************************************************************************************************
//                  maxon motor ag, CH-6072 Sachseln
// *******************************************************************************************************************
// Copyright © 2003 - 2021, maxon motor ag.
// All rights reserved.
// *******************************************************************************************************************

#include "definitions.h"

const int DM_PROGRESS_DLG                   = 0;
const int DM_PROGRESS_AND_CONFIRM_DLG       = 1;
const int DM_CONFIRM_DLG                    = 2;
const int DM_NO_DLG                         = 3;

//Configuration
//MotorType
const unsigned short MT_DC_MOTOR                      = 1;
const unsigned short MT_EC_SINUS_COMMUTATED_MOTOR     = 10;
const unsigned short MT_EC_BLOCK_COMMUTATED_MOTOR     = 11;

//SensorType
const unsigned short ST_UNKNOWN                       = 0;
const unsigned short ST_INC_ENCODER_3CHANNEL          = 1;
const unsigned short ST_INC_ENCODER_2CHANNEL          = 2;
const unsigned short ST_HALL_SENSORS                  = 3;
const unsigned short ST_SSI_ABS_ENCODER_BINARY        = 4;
const unsigned short ST_SSI_ABS_ENCODER_GREY          = 5;
const unsigned short ST_INC_ENCODER2_3CHANNEL         = 6;
const unsigned short ST_INC_ENCODER2_2CHANNEL         = 7;
const unsigned short ST_ANALOG_INC_ENCODER_3CHANNEL   = 8;
const unsigned short ST_ANALOG_INC_ENCODER_2CHANNEL   = 9;

//In- and outputs
//Digital input configuration
const unsigned short DIC_NEGATIVE_LIMIT_SWITCH        = 0;
const unsigned short DIC_POSITIVE_LIMIT_SWITCH        = 1;
const unsigned short DIC_HOME_SWITCH                  = 2;
const unsigned short DIC_POSITION_MARKER              = 3;
const unsigned short DIC_DRIVE_ENABLE                 = 4;
const unsigned short DIC_QUICK_STOP                   = 5;
const unsigned short DIC_GENERAL_PURPOSE_J            = 6;
const unsigned short DIC_GENERAL_PURPOSE_I            = 7;
const unsigned short DIC_GENERAL_PURPOSE_H            = 8;
const unsigned short DIC_GENERAL_PURPOSE_G            = 9;
const unsigned short DIC_GENERAL_PURPOSE_F            = 10;
const unsigned short DIC_GENERAL_PURPOSE_E            = 11;
const unsigned short DIC_GENERAL_PURPOSE_D            = 12;
const unsigned short DIC_GENERAL_PURPOSE_C            = 13;
const unsigned short DIC_GENERAL_PURPOSE_B            = 14;
const unsigned short DIC_GENERAL_PURPOSE_A            = 15;

//Digital output configuration
const unsigned short DOC_READY_FAULT                  = 0;
const unsigned short DOC_POSITION_COMPARE             = 1;
const unsigned short DOC_GENERAL_PURPOSE_H            = 8;
const unsigned short DOC_GENERAL_PURPOSE_G            = 9;
const unsigned short DOC_GENERAL_PURPOSE_F            = 10;
const unsigned short DOC_GENERAL_PURPOSE_E            = 11;
const unsigned short DOC_GENERAL_PURPOSE_D            = 12;
const unsigned short DOC_GENERAL_PURPOSE_C            = 13;
const unsigned short DOC_GENERAL_PURPOSE_B            = 14;
const unsigned short DOC_GENERAL_PURPOSE_A            = 15;

//Analog input configuration
const unsigned short AIC_ANALOG_CURRENT_SETPOINT      = 0;
const unsigned short AIC_ANALOG_VELOCITY_SETPOINT     = 1;
const unsigned short AIC_ANALOG_POSITION_SETPOINT     = 2;
const unsigned short AIC_GENERAL_PURPOSE_H            = 8;
const unsigned short AIC_GENERAL_PURPOSE_G            = 9;
const unsigned short AIC_GENERAL_PURPOSE_F            = 10;
const unsigned short AIC_GENERAL_PURPOSE_E            = 11;
const unsigned short AIC_GENERAL_PURPOSE_D            = 12;
const unsigned short AIC_GENERAL_PURPOSE_C            = 13;
const unsigned short AIC_GENERAL_PURPOSE_B            = 14;
const unsigned short AIC_GENERAL_PURPOSE_A            = 15;

//Analog output configuration
const unsigned short AOC_GENERAL_PURPOSE_A            = 0;
const unsigned short AOC_GENERAL_PURPOSE_B            = 1;

//Units
//VelocityDimension
const unsigned char VD_RPM                            = 0xA4;

//VelocityNotation
const signed char VN_STANDARD                         = 0;
const signed char VN_DECI                             = -1;
const signed char VN_CENTI                            = -2;
const signed char VN_MILLI                            = -3;

//Operational mode
const signed char OMD_PROFILE_POSITION_MODE          = 1;
const signed char OMD_PROFILE_VELOCITY_MODE          = 3;
const signed char OMD_HOMING_MODE                    = 6;
const signed char OMD_INTERPOLATED_POSITION_MODE     = 7;
const signed char OMD_POSITION_MODE                  = -1;
const signed char OMD_VELOCITY_MODE                  = -2;
const signed char OMD_CURRENT_MODE                   = -3;
const signed char OMD_MASTER_ENCODER_MODE            = -5;
const signed char OMD_STEP_DIRECTION_MODE            = -6;

//States
const unsigned short ST_DISABLED                         = 0;
const unsigned short ST_ENABLED                          = 1;
const unsigned short ST_QUICKSTOP                        = 2;
const unsigned short ST_FAULT                            = 3;

//Homing mode
//Homing method
const char HM_ACTUAL_POSITION                               = 35;
const char HM_NEGATIVE_LIMIT_SWITCH                         = 17;
const char HM_NEGATIVE_LIMIT_SWITCH_AND_INDEX               = 1;
const char HM_POSITIVE_LIMIT_SWITCH                         = 18;
const char HM_POSITIVE_LIMIT_SWITCH_AND_INDEX               = 2;
const char HM_HOME_SWITCH_POSITIVE_SPEED                    = 23;
const char HM_HOME_SWITCH_POSITIVE_SPEED_AND_INDEX          = 7;
const char HM_HOME_SWITCH_NEGATIVE_SPEED                    = 27;
const char HM_HOME_SWITCH_NEGATIVE_SPEED_AND_INDEX          = 11;
const char HM_CURRENT_THRESHOLD_POSITIVE_SPEED              = -3;
const char HM_CURRENT_THRESHOLD_POSITIVE_SPEED_AND_INDEX    = -1;
const char HM_CURRENT_THRESHOLD_NEGATIVE_SPEED              = -4;
const char HM_CURRENT_THRESHOLD_NEGATIVE_SPEED_AND_INDEX    = -2;
const char HM_INDEX_POSITIVE_SPEED                          = 34;
const char HM_INDEX_NEGATIVE_SPEED                          = 33;

//Input Output PositionMarker
//PositionMarkerEdgeType
const unsigned char PET_BOTH_EDGES                   = 0;
const unsigned char PET_RISING_EDGE                  = 1;
const unsigned char PET_FALLING_EDGE                 = 2;

//PositionMarkerMode
const unsigned char PM_CONTINUOUS                    = 0;
const unsigned char PM_SINGLE                        = 1;
const unsigned char PM_MULTIPLE                      = 2;

//Input Output PositionCompare
//PositionCompareOperationalMode
const unsigned char PCO_SINGLE_POSITION_MODE         = 0;
const unsigned char PCO_POSITION_SEQUENCE_MODE       = 1;

//PositionCompareIntervalMode
const unsigned char PCI_NEGATIVE_DIR_TO_REFPOS       = 0;
const unsigned char PCI_POSITIVE_DIR_TO_REFPOS       = 1;
const unsigned char PCI_BOTH_DIR_TO_REFPOS           = 2;

//PositionCompareDirectionDependency
const unsigned char PCD_MOTOR_DIRECTION_NEGATIVE     = 0;
const unsigned char PCD_MOTOR_DIRECTION_POSITIVE     = 1;
const unsigned char PCD_MOTOR_DIRECTION_BOTH         = 2;

//Data recorder
//Trigger type
const unsigned short DR_MOVEMENT_START_TRIGGER        = 1;    //bit 1
const unsigned short DR_ERROR_TRIGGER                 = 2;    //bit 2
const unsigned short DR_DIGITAL_INPUT_TRIGGER         = 4;    //bit 3
const unsigned short DR_MOVEMENT_END_TRIGGER          = 8;    //bit 4

//CanLayer Functions
const unsigned short NCS_START_REMOTE_NODE            = 1;
const unsigned short NCS_STOP_REMOTE_NODE             = 2;
const unsigned short NCS_ENTER_PRE_OPERATIONAL        = 128;
const unsigned short NCS_RESET_NODE                   = 129;
const unsigned short NCS_RESET_COMMUNICATION          = 130;

// Controller Gains
// EController
const unsigned short EC_PI_CURRENT_CONTROLLER                   = 1;
const unsigned short EC_PI_VELOCITY_CONTROLLER                  = 10;
const unsigned short EC_PI_VELOCITY_CONTROLLER_WITH_OBSERVER    = 11;
const unsigned short EC_PID_POSITION_CONTROLLER                 = 20;
const unsigned short EC_DUAL_LOOP_POSITION_CONTROLLER           = 21;

// EGain (EC_PI_CURRENT_CONTROLLER)
const unsigned short EG_PICC_P_GAIN                             = 1;
const unsigned short EG_PICC_I_GAIN                             = 2;

// EGain (EC_PI_VELOCITY_CONTROLLER)
const unsigned short EG_PIVC_P_GAIN                             = 1;
const unsigned short EG_PIVC_I_GAIN                             = 2;
const unsigned short EG_PIVC_FEED_FORWARD_VELOCITY_GAIN         = 10;
const unsigned short EG_PIVC_FEED_FORWARD_ACCELERATION_GAIN     = 11;

// EGain (EC_PI_VELOCITY_CONTROLLER_WITH_OBSERVER)
const unsigned short EG_PIVCWO_P_GAIN                           = 1;
const unsigned short EG_PIVCWO_I_GAIN                           = 2;
const unsigned short EG_PIVCWO_FEED_FORWARD_VELOCITY_GAIN       = 10;
const unsigned short EG_PIVCWO_FEED_FORWARD_ACCELERATION_GAIN   = 11;
const unsigned short EG_PIVCWO_OBSERVER_THETA_GAIN              = 20;
const unsigned short EG_PIVCWO_OBSERVER_OMEGA_GAIN              = 21;
const unsigned short EG_PIVCWO_OBSERVER_TAU_GAIN                = 22;

// EGain (EC_PID_POSITION_CONTROLLER)
const unsigned short EG_PIDPC_P_GAIN                            = 1;
const unsigned short EG_PIDPC_I_GAIN                            = 2;
const unsigned short EG_PIDPC_D_GAIN                            = 3;
const unsigned short EG_PIDPC_FEED_FORWARD_VELOCITY_GAIN        = 10;
const unsigned short EG_PIDPC_FEED_FORWARD_ACCELERATION_GAIN    = 11;

// EGain (EC_DUAL_LOOP_POSITION_CONTROLLER)
const unsigned short EG_DLPC_AUXILIARY_LOOP_P_GAIN                          = 1;
const unsigned short EG_DLPC_AUXILIARY_LOOP_I_GAIN                          = 2;
const unsigned short EG_DLPC_AUXILIARY_LOOP_FEED_FORWARD_VELOCITY_GAIN      = 10;
const unsigned short EG_DLPC_AUXILIARY_LOOP_FEED_FORWARD_ACCELERATION_GAIN  = 11;
const unsigned short EG_DLPC_AUXILIARY_LOOP_OBSERVER_THETA_GAIN             = 20;
const unsigned short EG_DLPC_AUXILIARY_LOOP_OBSERVER_OMEGA_GAIN             = 21;
const unsigned short EG_DLPC_AUXILIARY_LOOP_OBSERVER_TAU_GAIN               = 22;
const unsigned short EG_DLPC_MAIN_LOOP_P_GAIN_LOW                           = 101;
const unsigned short EG_DLPC_MAIN_LOOP_P_GAIN_HIGH                          = 102;
const unsigned short EG_DLPC_MAIN_LOOP_GAIN_SCHEDULING_WEIGHT               = 110;
const unsigned short EG_DLPC_MAIN_LOOP_FILTER_COEFFICIENT_A                 = 120;
const unsigned short EG_DLPC_MAIN_LOOP_FILTER_COEFFICIENT_B                 = 121;
const unsigned short EG_DLPC_MAIN_LOOP_FILTER_COEFFICIENT_C                 = 122;
const unsigned short EG_DLPC_MAIN_LOOP_FILTER_COEFFICIENT_D                 = 123;
const unsigned short EG_DLPC_MAIN_LOOP_FILTER_COEFFICIENT_E                 = 124;
