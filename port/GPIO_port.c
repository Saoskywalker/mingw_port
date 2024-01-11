/*
Copyright (c) 2019-2023 Aysi 773917760@qq.com. All right reserved
Official site: www.mtf123.club

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

It under the terms of the Apache as published;
either version 2 of the License,
or (at your option) any later version.
*/

#include "GPIO_port.h"

MTF_GPIO_PORT GPIO_PA[16];
MTF_GPIO_PORT GPIO_PB[16];
MTF_GPIO_PORT GPIO_PC[16];
MTF_GPIO_PORT GPIO_PD[16];
MTF_GPIO_PORT GPIO_PE[16];
MTF_GPIO_PORT GPIO_PF[16];

void MTF_GPIO_Init(MTF_GPIO_PORT *GPIOx, MTF_GPIO_Type *GPIO_Init)
{
    GPIOx[GPIO_Init->Pin].direction = GPIO_Init->Mode;
}
