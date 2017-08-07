#include "main.h"
#include "string.h"

extern PID_Type *Motor_X;
extern PID_Type *Motor_Y;


#define PositionPitch_P 0.0f
#define PositionPitch_I 0.0f
#define PositionPitch_D 0.0f

#define SpeedPitch_P 0.0f
#define SpeedPitch_I 0.0f
#define SpeedPitch_D 0.0f

#define PositionRoll_P 0.0f
#define PositionRoll_I 0.0f
#define PositionRoll_D 0.0f

#define SpeedRoll_P 0.0f
#define SpeedRoll_I 0.0f
#define SpeedRoll_D 0.0f

#define Motor_P 0.0f
#define Motor_I 0.0f
#define Motor_D 0.0f
union _Pid_un_ pid_un;


void Param_SaveAccelOffset(xyz_f_t *offset)
{
 memcpy(&mpu6050.Acc_Offset,offset,sizeof(xyz_f_t));
 memcpy(&sensor_setup.Offset.Accel, offset,sizeof(xyz_f_t));
	
 sensor_setup.Offset.Acc_Temperature = mpu6050.Acc_Temprea_Offset ;
}

void Param_SaveGyroOffset(xyz_f_t *offset)
{
 memcpy(&mpu6050.Gyro_Offset,offset,sizeof(xyz_f_t));
 memcpy(&sensor_setup.Offset.Gyro, offset,sizeof(xyz_f_t));
	
 sensor_setup.Offset.Gyro_Temperature = mpu6050.Gyro_Temprea_Offset ;
}

void Param_SaveMagOffset(xyz_f_t *offset)
{
 memcpy(&ak8975.Mag_Offset,offset,sizeof(xyz_f_t));
 memcpy(&sensor_setup.Offset.Mag, offset,sizeof(xyz_f_t));
}

void Para_ResetToFactorySetup(void)
{
	return;
}

u8 pid_saved_flag=0;
uint8_t AppParamSave(void)
{
//	pid_saved_flag=1;
//	BSP_FLASH_Write(PARAM_SAVED_FLAG_ADDRESS, &pid_saved_flag, 1);
   return 0;
}

