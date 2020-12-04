/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: Yang Xiang (Shawn)
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
/*
 * =====================================================================================
 *
 *       Filename:  MyMath.c
 *
 *    Description:  一些常用的数学函数
 *
 *        Version:  1.0
 *        Created:  2015/9/16 17:30:47
 *       Revision:  none
 *       Compiler:  Keil ARM-MDK 4.60
 *
 *         Author:  Yang Xiang (Shawn), yangxiang92@163.com
 *        Company:  Insitute of Automation, Chinese Academy of Sciences
 *
 * Modification History:
 * 01a 2015/9/16 17:30:47
 *
 * =====================================================================================
 */
#include "MyMath.h"


/*
 * ===  FUNCTION  ======================================================================
 *
 *         Name: MyMath_Cal3DModulus
 *  Description: 计算三维的模量，也就是计算三维向量的长度
 *
 * Modification History:
 * 01a 2015/9/16 17:33:09
 *
 * =====================================================================================
 */
float MyMath_Cal3DModulus ( float x, float y, float z )
{
    float modulus = 0.0f;
    float square_sum = 0.0f;

    square_sum += x*x;
    square_sum += y*y;
    square_sum += z*z;

    modulus = sqrt(square_sum);

    return modulus;
}       /* -----  end of function MyMath_Cal3DModulus  ----- */

/*
 * ===  FUNCTION  ======================================================================
 *
 *         Name: MyMath_ApplyDeadBandF32
 *  Description: 对float型的数据进行死区处理
 *
 * Modification History:
 * 01a 2015/9/17 22:03:04
 *
 * =====================================================================================
 */
float MyMath_ApplyDeadBandF32 ( float data, float dead_band )
{
    // 首先得到dead_band的绝对值
    if(dead_band < 0)
    {
        dead_band = -dead_band;
    }

    // 在死区内则置为0
    if(data > -dead_band && data < dead_band)
    {
        data = 0.0f;
    }
    else
    {
        // Do nothing;
    }

    return data;
}       /* -----  end of function MyMath_ApplyDeadBandF32  ----- */

/*
 * ===  FUNCTION  ======================================================================
 *
 *         Name: MyMath_ConstrainF32
 *  Description: 对float型数据进行最值限制
 *
 * Modification History:
 * 01a 2015/9/17 22:10:19
 *
 * =====================================================================================
 */
float MyMath_ConstrainF32 ( float data, float low_limit, float high_limit )
{
    float temp = 0.0f;

    // 如果低值高于高值的话，交换他们就可以得到正常的处理流程
    if(low_limit > high_limit)
    {
        temp = low_limit;
        low_limit = high_limit;
        high_limit = temp;
    }

    if(data < low_limit)
    {
        data = low_limit;
    }
    else if(data > high_limit)
    {
        data = high_limit;
    }
    else
    {
        // Do nothing;
    }

    return data;
}       /* -----  end of function MyMath_ConstrainF32  ----- */

/** 
 * @brief  Sign funtion of float number
 * 
 * @param  f
 * 
 * @retval Sign(float)
 */
float MyMath_SignF32(float f){
    if(f >= 0.0f){
        return 1.0f;
    }else{
        return -1.0f;
    }
}

/** 
 * @brief  Convert float to int
 * 
 * @param  target
 * 
 * @retval 
 */
int MyMath_FloatToInt(float target){
    int c, f;
    c = ceil(target);
    f = floor(target);
    if(fabs(target - c) > fabs(target - f)){
        return f;
    }else{
        return c;
    }
}
