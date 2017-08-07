#include "main.h"

PID_Type* Motor_X;
PID_Type* Motor_Y;

void set_pid(PID_Type* PID,float kp,float ki,float kd)
{
	PID->kp=kp;
	PID->ki=ki;
	PID->kd=kd;
	PID->errNow=PID->errOld1=PID->errOld2=0;
	PID->output=0;
}

void pid_cal(PID_Type* PID)
{
  float dErrP, dErrI, dErrD;
	PID->errNow=PID->ref-PID->now;  
  dErrP = (PID->errNow - PID->errOld1);
  dErrI = PID->errNow;
  dErrD = (PID->errNow - 2.0f * PID->errOld1 + PID->errOld2);
  PID->doutput = (PID->kp * dErrP + PID->ki * dErrI + PID->kd * dErrD);
  PID->output += PID->doutput;//Õ¼¿Õ±È
	PID->errOld2 = PID->errOld1; 
  PID->errOld1 = PID->errNow;  
	if((PID->output)>DUTY_MAX) PID->output=DUTY_MAX;
	if((PID->output)<-DUTY_MAX) PID->output=-DUTY_MAX;
}

/*------------------------------------------
KALMAN  pitch
------------------------------------------*/
//float Kalman_Filter1(float Accel,float Gyro)		
//{
//	static float Gyro_y;   //Y????????
//	static float angle = 0.0;
//	static float Q_bias = 0.0;
//	static float angle_err = 0.0;
//	static float Q_angle = 0.1;  //???????
//	static float Q_gyro = 0.1;   //????????
//	static float R_angle = 0.5;
//	static float dt = 0.005;	   //dt????????(?)
//	static char  C_0 = 1;
//	static float PCt_0=0, PCt_1=0, E=0;
//	static float K_0=0, K_1=0, t_0=0, t_1=0;
//	static float Pdot[4] ={0,0,0,0};
//	static float PP[2][2] = { { 1, 0 },{ 0, 1 } };

//	angle += (Gyro - Q_bias) * dt; //????
//	angle_err = Accel - angle;	//zk-????

//	Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-????????????
//	Pdot[1]-= PP[1][1];
//	Pdot[2]-= PP[1][1];
//	Pdot[3]=Q_gyro;
//	PP[0][0] += Pdot[0] * dt;   // Pk-??????????????
//	PP[0][1] += Pdot[1] * dt;   // =?????????
//	PP[1][0] += Pdot[2] * dt;
//	PP[1][1] += Pdot[3] * dt;	

//	PCt_0 = C_0 * PP[0][0];
//	PCt_1 = C_0 * PP[1][0];
//	
//	E = R_angle + C_0 * PCt_0;
//	
//	K_0 = PCt_0 / E;
//	K_1 = PCt_1 / E;
//	
//	t_0 = PCt_0;
//	t_1 = C_0 * PP[0][1];

//	PP[0][0] -= K_0 * t_0;		 //?????????
//	PP[0][1] -= K_0 * t_1;
//	PP[1][0] -= K_1 * t_0;
//	PP[1][1] -= K_1 * t_1;
//		
//	angle += K_0 * angle_err;	 //????
//	Q_bias += K_1 * angle_err;	 
//	Gyro_y = Gyro - Q_bias;	 //?????

//	return (angle);				 //????			 
//}

///*------------------------------------------
// roll
//------------------------------------------*/
//float Kalman_Filter2(float Accel,float Gyro)		
//{
//	static float Gyro_y;   //Y????????
//	static float angle = 0.0;
//	static float Q_bias = 0.0;
//	static float angle_err = 0.0;
//	static float Q_angle = 0.1;  //???????
//	static float Q_gyro = 0.1;   //????????
//	static float R_angle = 0.5;
//	static float dt = 0.005;	   //dt????????(?)
//	static char  C_0 = 1;
//	static float PCt_0=0, PCt_1=0, E=0;
//	static float K_0=0, K_1=0, t_0=0, t_1=0;
//	static float Pdot[4] ={0,0,0,0};
//	static float PP[2][2] = { { 1, 0 },{ 0, 1 } };

//	angle += (Gyro - Q_bias) * dt; //????
//	angle_err = Accel - angle;	//zk-????

//	Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-????????????
//	Pdot[1]=- PP[1][1];
//	Pdot[2]=- PP[1][1];
//	Pdot[3]=Q_gyro;
//	PP[0][0] += Pdot[0] * dt;   // Pk-??????????????
//	PP[0][1] += Pdot[1] * dt;   // =?????????
//	PP[1][0] += Pdot[2] * dt;
//	PP[1][1] += Pdot[3] * dt;	

//	PCt_0 = C_0 * PP[0][0];
//	PCt_1 = C_0 * PP[1][0];
//	
//	E = R_angle + C_0 * PCt_0;
//	
//	K_0 = PCt_0 / E;
//	K_1 = PCt_1 / E;
//	
//	t_0 = PCt_0;
//	t_1 = C_0 * PP[0][1];

//	PP[0][0] -= K_0 * t_0;		 //?????????
//	PP[0][1] -= K_0 * t_1;
//	PP[1][0] -= K_1 * t_0;
//	PP[1][1] -= K_1 * t_1;
//		
//	angle += K_0 * angle_err;	 //????
//	Q_bias += K_1 * angle_err;	 
//	Gyro_y = Gyro - Q_bias;	 //?????

//	return (angle);				 //????			 
//}

