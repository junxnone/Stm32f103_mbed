/* mbed Microcontroller Library
 * Copyright (c) 2014, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "mbed_assert.h"
#include "analogout_api.h"

#if DEVICE_ANALOGOUT

#include "cmsis.h"
#include "pinmap.h"
#include "mbed_error.h"

#define DAC_RANGE (0xFFF) // 12 bits

static const PinMap PinMap_DAC[] = {
    {PA_4, DAC_1, STM_PIN_DATA(GPIO_Mode_AIN, 0)}, // DAC_OUT
    {PA_5, DAC_1, STM_PIN_DATA(GPIO_Mode_AIN, 0)}, // DAC_OUT
	{NC,   NC,    0}
};

static DAC_InitTypeDef DAC_InitType;

void analogout_init(dac_t *obj, PinName pin) {

    // Get the peripheral name (DAC_1, ...) from the pin and assign it to the object
   obj->dac = (DACName)pinmap_peripheral(pin, PinMap_DAC);
   
   MBED_ASSERT(obj->dac != (DACName)NC);
	
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE );
    // Configure GPIO
    pinmap_pinout(pin, PinMap_DAC);

    // Save the channel for future use
    obj->pin = pin; 

    // Configure DAC
    DAC_InitType.DAC_Trigger      = DAC_Trigger_None;
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;
    DAC_InitType.DAC_OutputBuffer = DAC_OutputBuffer_Disable;

   // HAL_DAC_ConfigChannel(&DAC_InitType, &sConfig, DAC_CHANNEL_1);
	if(obj->pin == PA_4)
	{
	  DAC_Init(DAC_Channel_1,&DAC_InitType);	 //初始化DAC通道1
	  DAC_Cmd(DAC_Channel_1, ENABLE);
	}
	if(obj->pin == PA_5)
	{
	  DAC_Init(DAC_Channel_2,&DAC_InitType);	 //初始化DAC通道2
	  DAC_Cmd(DAC_Channel_2, ENABLE);
	}
    analogout_write_u16(obj, 0);
}

void analogout_free(dac_t *obj) {
    // Reset DAC and disable clock
    DAC_DeInit();
    // Configure GPIO
    pin_function(obj->pin, STM_PIN_DATA(GPIO_Mode_AIN, 0));
}

static inline void dac_write(dac_t *obj, uint16_t value) {
   // HAL_DAC_SetValue(&DAC_InitType, DAC_CHANNEL_1, DAC_ALIGN_12B_R, value);
	if(obj->pin == PA_4)
	DAC_SetChannel1Data(DAC_Align_12b_R, value);
	if(obj->pin == PA_5)
	DAC_SetChannel2Data(DAC_Align_12b_R, value);
   // HAL_DAC_Start(&DAC_InitType, DAC_1);
}

static inline int dac_read(dac_t *obj) {
	if(obj->pin == PA_4)
    return (int)DAC_GetDataOutputValue(DAC_Channel_1);
	else if(obj->pin == PA_5)
    return (int)DAC_GetDataOutputValue(DAC_Channel_2);
	else return 0;
}

void analogout_write(dac_t *obj, float value) {
    if (value < 0.0f) {
        dac_write(obj, 0); // Min value
    } else if (value > 1.0f) {
        dac_write(obj, (uint16_t)DAC_RANGE); // Max value
    } else {
        dac_write(obj, (uint16_t)(value * (float)DAC_RANGE));
    }
}

void analogout_write_u16(dac_t *obj, uint16_t value) {
    if (value > (uint16_t)DAC_RANGE) {
        dac_write(obj, (uint16_t)DAC_RANGE); // Max value
    } else {
        dac_write(obj, value);
    }
}

float analogout_read(dac_t *obj) {
    uint32_t value = dac_read(obj);
    return (float)((float)value * (1.0f / (float)DAC_RANGE));
}

uint16_t analogout_read_u16(dac_t *obj) {
    return (uint16_t)dac_read(obj);
}

#endif // DEVICE_ANALOGOUT
