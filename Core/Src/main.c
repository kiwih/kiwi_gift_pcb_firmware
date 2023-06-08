/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "usbd_cdc_if.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

#define LED_BRIGHTNESS_CYCLES 40

//an array of structs where the struct contains indexes in the array and a length
typedef struct {
	uint8_t len;
	uint8_t indexes[3];
} led_links_t;
//
led_links_t led_links[] = {
		{1, {1}},    //led 1 links to led 2 only
		{2, {0, 2}}, //led 2 links to led 1 and 3
		{2, {1, 3}}, //led 3 links to led 2 and 4
		{2, {2, 4}}, //led 4 links to led 3 and 5
		{3, {3, 5, 8}}, //led 5 links to led 6 and 9
		{3, {4, 6, 8}}, //led 6 links to led 5, 6, and 9
		{2, {5, 7}}, //led 7 links to led 6 and 8
		{2, {6, 10}}, //led 8 links to led 7 and 11
		{3, {4, 5, 11}}, //led 9 links to led 5, 6, and 12
		{0, {0}}, //led 10 has no links
		{2, {7, 12}}, //led 11 has links to led 8 and 13
		{2, {8, 13}}, //led 12 links to led 9 and 14
		{2, {10, 14}}, //led 13 links to led 11 and 15
		{2, {11, 15}}, //led 14 links to led 12 and 16
		{2, {12, 16}}, //led 15 links to led 13 and 17
		{2, {13, 17}}, //led 16 links to led 14 and 18
		{2, {14, 18}}, //led 17 links to led 15 and 19
		{3, {15, 20, 23}}, //led 18 links to led 16, 21 and 24
		{2, {16, 21}}, //led 19 links to led 17 and 22
		{1, {20}}, //led 20 links to led 21
		{3, {17, 19, 23}}, //led 21 links to 18, 20, 24
		{2, {18, 24}}, //led 22 links to 18 and 25
		{1, {25}}, //led 23 links to 26
		{3, {17, 20, 26}}, //led 24 links to 18, 21, 27
		{2, {21, 28}}, //led 25 links to 22, 29
		{2, {22, 26}}, //led 26 links to 23, 28
		{3, {23, 25, 27}}, //led 27 links to 24, 26, 28
		{2, {26, 30}}, //led 28 links to 27, 31
		{2, {24, 29}}, //led 29 links to 25, 30
		{2, {28, 31}}, //led 30 links to 29, 32
		{2, {27, 32}}, //led 31 links to 28, 33
		{2, {29, 32}}, //led 32 links to 30, 33
		{2, {30, 31}}, //led 33 links to 31, 32
};

#define LEDS_N 33

typedef struct {
	GPIO_TypeDef *GPIOx;
	uint16_t GPIO_Pin;
	uint8_t brightness;
	int8_t velocity;
} led_status_t;

led_status_t leds_status[LEDS_N] = {
		{LED1_GPIO_Port, LED1_Pin, 0, 0},
		{LED2_GPIO_Port, LED2_Pin, 0, 0},
		{LED3_GPIO_Port, LED3_Pin, 0, 0},
		{LED4_GPIO_Port, LED4_Pin, 0, 0},
		{LED5_GPIO_Port, LED5_Pin, 0, 0},
		{LED6_GPIO_Port, LED6_Pin, 0, 0},
		{LED7_GPIO_Port, LED7_Pin, 0, 0},
		{LED8_GPIO_Port, LED8_Pin, 0, 0},
		{LED9_GPIO_Port, LED9_Pin, 0, 0},
		{LED10_GPIO_Port, LED10_Pin, 0, 0},
		{LED11_GPIO_Port, LED11_Pin, 0, 0},
		{LED12_GPIO_Port, LED12_Pin, 0, 0},
		{LED13_GPIO_Port, LED13_Pin, 0, 0},
		{LED14_GPIO_Port, LED14_Pin, 0, 0},
		{LED15_GPIO_Port, LED15_Pin, 0, 0},
		{LED16_GPIO_Port, LED16_Pin, 0, 0},
		{LED17_GPIO_Port, LED17_Pin, 0, 0},
		{LED18_GPIO_Port, LED18_Pin, 0, 0},
		{LED19_GPIO_Port, LED19_Pin, 0, 0},
		{LED20_GPIO_Port, LED20_Pin, 0, 0},
		{LED21_GPIO_Port, LED21_Pin, 0, 0},
		{LED22_GPIO_Port, LED22_Pin, 0, 0},
		{LED23_GPIO_Port, LED23_Pin, 0, 0},
		{LED24_GPIO_Port, LED24_Pin, 0, 0},
		{LED25_GPIO_Port, LED25_Pin, 0, 0},
		{LED26_GPIO_Port, LED26_Pin, 0, 0},
		{LED27_GPIO_Port, LED27_Pin, 0, 0},
		{LED28_GPIO_Port, LED28_Pin, 0, 0},
		{LED29_GPIO_Port, LED29_Pin, 0, 0},
		{LED30_GPIO_Port, LED30_Pin, 0, 0},
		{LED31_GPIO_Port, LED31_Pin, 0, 0},
		{LED32_GPIO_Port, LED32_Pin, 0, 0},
		{LED33_GPIO_Port, LED33_Pin, 0, 0},
};

void leds_set_brightness_all(uint8_t brightness) { //brightness is a number between 0 and 20 (0 == never on, 50 == always on)
	for(uint8_t i = 0; i < LEDS_N; i++) {
		leds_status[i].brightness = brightness;
	}
}

void leds_set_border(uint8_t brightness, int8_t velocity) {
	for(uint8_t i = 0; i < LEDS_N; i++) {
		if(i == 9) continue;
		leds_status[i].brightness = brightness;
		leds_status[i].velocity = velocity;
	}
}

void leds_set_eye(uint8_t brightness, int8_t velocity) {
	leds_status[9].brightness = brightness;
	leds_status[9].velocity = velocity;
}

uint8_t leds_get_random_index() {
	//return a number between 0 and LEDS_N-1 not including 10
	do {
		uint8_t index = rand() % LEDS_N;
		if(index != 9) {
			return index;
		}
	} while(1);
}


bool led_is_lit(uint8_t cycle, uint8_t led_index) { //cycle is a number between 1 and 50 (does not cycle 0)
	return (leds_status[led_index].brightness >= cycle);
}

void leds_emit(uint8_t cycle) {
	for(uint8_t i = 0; i < LEDS_N; i++) {
		HAL_GPIO_WritePin(leds_status[i].GPIOx, leds_status[i].GPIO_Pin, led_is_lit(cycle, i));
	}
}

//LEDS PROGRAM ONE:
//let's make a scrolling pattern using the led_links array
//starting from the first index, light subsequent leds in the array

void leds_program_one_fill_init(uint8_t first_led_index, uint8_t fill, int8_t velocity) {
	leds_set_border(fill, 0);
	leds_status[first_led_index].velocity = velocity;
	//leds_set_brightness_eye(fill);
	//leds_status[9].velocity = 0;
}

void leds_program_one_eye_init(uint8_t fill, int8_t velocity) {
	leds_set_eye(fill, velocity);
}

int8_t leds_program_one_single_led_step(uint8_t i) {
	int8_t delta = leds_status[i].velocity;
	//make sure brightness + delta is not greater than LED_BRIGHTNESS_CYCLES
	if(leds_status[i].brightness + delta > LED_BRIGHTNESS_CYCLES) {
		delta = LED_BRIGHTNESS_CYCLES - leds_status[i].brightness;
		leds_status[i].velocity = 0;
	}
	//make sure brightness + delta is not less than 0
	if(leds_status[i].brightness + delta < 0) {
		delta = -leds_status[i].brightness;
		leds_status[i].velocity = 0;
	}
	//increment brightness by delta
	leds_status[i].brightness += delta;
	return delta;
}


uint8_t leds_program_one_step() {
	//for each led which has a non-zero velocity, update its brightness to bounds [0, LED_BRIGHTNESS_CYCLES]
	//then, for each led with brightness crossing the midpoint LED_BRIGHTNESS_CYCLES/2, update the velocity of the neighbors
	//return the number of LEDs that changed in this cycle
	uint8_t n_changed = 0;

	for(uint8_t i = 0; i < LEDS_N; i++) {
		if(leds_status[i].velocity != 0) {
				
			if(leds_program_one_single_led_step(i) != 0)
				n_changed++;

			//if we are past the midpoint (velocity is positive and brightness is at least 10,
			// or velocity is negative and brightness is at most 10), propagate the velocity to linked leds
			if((leds_status[i].velocity > 0 && leds_status[i].brightness >= LED_BRIGHTNESS_CYCLES/2) ||
			   (leds_status[i].velocity < 0 && leds_status[i].brightness <= LED_BRIGHTNESS_CYCLES - (LED_BRIGHTNESS_CYCLES/2))) {
				for(uint8_t j = 0; j < led_links[i].len; j++) {
					if(leds_status[i].velocity > 0 && leds_status[led_links[i].indexes[j]].brightness == 0) {
						leds_status[led_links[i].indexes[j]].velocity = leds_status[i].velocity;
					}
					if(leds_status[i].velocity < 0 && leds_status[led_links[i].indexes[j]].brightness == LED_BRIGHTNESS_CYCLES) {
						leds_status[led_links[i].indexes[j]].velocity = leds_status[i].velocity;
					}
				}
			}
		}
	}
	return n_changed;
}

//LEDS PROGRAM TWO:
//all LEDs are illuminated evenly and then fade evenly (all outer ON, eye ON, eye OFF, all outer OFF)

void leds_program_two_border_init(uint8_t fill, int8_t velocity) {
	leds_set_border(fill, velocity);
}

void leds_program_two_eye_init(uint8_t fill, int8_t velocity) {
	leds_set_eye(fill, velocity); //reuse program one since it's much the same
}

int8_t leds_program_two_single_led_step(uint8_t i) {
	return leds_program_one_single_led_step(i);
}

uint8_t leds_program_two_step() {
	//for each led which has a brightness of at least 1, increment its brightness by 1 to a maximum of 20
	//then, for each led with brightness at least 10, set the brightness of its linked leds to 1 if they are not already at least 1
	//return the number of LEDs that changed in this cycle
	uint8_t n_changed = 0;
	//uint8_t turn_on_off[LEDS_N] = {0};

	for(uint8_t i = 0; i < LEDS_N; i++) {
		if(leds_status[i].velocity != 0) {
			if(leds_program_one_single_led_step(i) != 0)
				n_changed++;
		}
	}
	return n_changed;
}

int8_t rand_small_nonzero_velocity() {
	//return a random number between -3 and 3, not including 0
	int8_t velocity = 0;
	while(velocity == 0) {
		velocity = (rand() % 7) - 3;
	}
	return velocity;
}

void leds_program_three_init(uint8_t fadeout) { //1 == fadeout, 0 == first startup
	for(uint8_t i = 0; i < LEDS_N; i++) {
		if(fadeout == 0) {
			leds_status[i].brightness = 0;
			leds_status[i].velocity = (rand() % 4); //first startup: no negative velocities to start with, but some zeros
		} else {
			leds_status[i].velocity = (rand() % 2) - 2; //fadeout: no positive velocities, no zero velocities
		}
	}
}

void leds_program_three_bounce_step() {
	for(uint8_t i = 0; i < LEDS_N; i++) {
		//if(leds_status[i].velocity != 0) {
			if(leds_program_one_single_led_step(i) == 0) {
				//sometimes we'll get negative velocities for something set to zero
				//or positive ones for those set to max
				//meaning they'll get a random delay the next cycle instead
				//that's okay, they'll get a new velocity next cycle
				leds_status[i].velocity = rand_small_nonzero_velocity();
			}
		//}
	}
}

uint8_t leds_program_three_fade_out_step() {
	return leds_program_two_step();
}

#define BASE_AWAIT_CYCLE_COUNT (LED_BRIGHTNESS_CYCLES * 1)

enum program_state_e {
	program_one_fill_init,
	program_one_fill,
	program_one_fill_eye,
	program_one_unfill_init,
	program_one_unfill,
	program_one_unfill_eye,

	program_two_fill_init,
	program_two_fill,
	program_two_fill_eye,
	program_two_unfill_init,
	program_two_unfill_eye,
	program_two_unfill,

	program_three_init,
	program_three_bounce,
	program_three_fadeout
};

void advance_state(int* program_repeat_count, enum program_state_e *program_state, uint32_t cycle_count, uint32_t *await_cycle_count) {
	switch(*program_state) {
	case program_one_fill_init:
		leds_program_one_fill_init(leds_get_random_index(), 0, 1);
		*await_cycle_count = BASE_AWAIT_CYCLE_COUNT * 6;
		*program_state = program_one_fill;
		break;
	case program_one_fill:
		if(leds_program_one_step() == 0) {
			leds_program_one_eye_init(0, 1);
			*program_state = program_one_fill_eye;
			*await_cycle_count = 0;
		}
		break;
	case program_one_fill_eye:
		*await_cycle_count = BASE_AWAIT_CYCLE_COUNT * 10;
		if(leds_program_one_single_led_step(9) == 0) {
			*await_cycle_count = BASE_AWAIT_CYCLE_COUNT * 100;
			if(cycle_count == BASE_AWAIT_CYCLE_COUNT * 100) {
				*program_state = program_one_unfill_init;
				*await_cycle_count = 0;
			}
		}
		break;
	case program_one_unfill_init:
			leds_program_one_fill_init(leds_get_random_index(), LED_BRIGHTNESS_CYCLES, -1);
			*await_cycle_count = BASE_AWAIT_CYCLE_COUNT * 6;
			*program_state = program_one_unfill;
			break;
	case program_one_unfill:
		if(leds_program_one_step() == 0) {
			leds_program_one_eye_init(LED_BRIGHTNESS_CYCLES, -1);
			*program_state = program_one_unfill_eye;
			*await_cycle_count = 0;
		}
		break;
	case program_one_unfill_eye:
		*await_cycle_count = BASE_AWAIT_CYCLE_COUNT * 10;
		if(leds_program_one_single_led_step(9) == 0) {
			*await_cycle_count = BASE_AWAIT_CYCLE_COUNT * 100;
			if(cycle_count == BASE_AWAIT_CYCLE_COUNT * 100) {
				if(*program_repeat_count < 3) {
					*program_state = program_one_fill_init;
					(*program_repeat_count)++;
				} else {
					*program_state = program_two_fill_init;
					(*program_repeat_count)=0;
				}
				*await_cycle_count = 0;
			}
		}
		break;

	case program_two_fill_init:
		leds_program_two_border_init(0, 1);
		leds_program_two_eye_init(0,0);
		*await_cycle_count = BASE_AWAIT_CYCLE_COUNT * 6;
		*program_state = program_two_fill;
		break;
	case program_two_fill:
		if(leds_program_two_step() == 0) {
			leds_program_two_eye_init(0, 1);
			*program_state = program_two_fill_eye;
			*await_cycle_count = 0;
		}
		break;
	case program_two_fill_eye:
		*await_cycle_count = BASE_AWAIT_CYCLE_COUNT * 10;
		if(leds_program_one_single_led_step(9) == 0) {
			*await_cycle_count = BASE_AWAIT_CYCLE_COUNT * 100;
			if(cycle_count == BASE_AWAIT_CYCLE_COUNT * 100) {
				*program_state = program_two_unfill_init;
				*await_cycle_count = 0;
			}
		}
		break;
	case program_two_unfill_init:
			leds_program_two_border_init(LED_BRIGHTNESS_CYCLES, -1);
			leds_program_two_eye_init(LED_BRIGHTNESS_CYCLES, -1);
			*await_cycle_count = BASE_AWAIT_CYCLE_COUNT * 6;
			*program_state = program_two_unfill_eye;
			break;
	case program_two_unfill_eye:
		*await_cycle_count = BASE_AWAIT_CYCLE_COUNT * 10;
		if(leds_program_one_single_led_step(9) == 0) {
			//*await_cycle_count = BASE_AWAIT_CYCLE_COUNT * 100;
			//if(cycle_count == BASE_AWAIT_CYCLE_COUNT * 100) {
				*program_state = program_two_unfill;
			//}
		}
		break;
	case program_two_unfill:
		if(leds_program_two_step() == 0) {
			*await_cycle_count = BASE_AWAIT_CYCLE_COUNT * 100;
			if(cycle_count == BASE_AWAIT_CYCLE_COUNT * 100) {
				if(*program_repeat_count < 1) {
					*program_state = program_two_fill_init;
					(*program_repeat_count)++;
				} else {
					*program_state = program_three_init;
					(*program_repeat_count)=0;
				}
				*await_cycle_count = 0;
			}
		}
		break;

	case program_three_init:
		leds_program_three_init(0);
		*await_cycle_count = BASE_AWAIT_CYCLE_COUNT * 3;
		*program_state = program_three_bounce;
		break;
	case program_three_bounce:
		leds_program_three_bounce_step();
		*await_cycle_count = BASE_AWAIT_CYCLE_COUNT * 10;
		if(*program_repeat_count < 400) {
			*program_state = program_three_bounce;
			(*program_repeat_count)++;
		} else {
			leds_program_three_init(1);
			*program_state = program_three_fadeout;
			(*program_repeat_count)=0;
		}
		break;
	case program_three_fadeout:
		if(leds_program_three_fade_out_step() == 0) {
			*await_cycle_count = BASE_AWAIT_CYCLE_COUNT * 400;
			if(cycle_count == BASE_AWAIT_CYCLE_COUNT * 400) {
				*program_state = program_one_fill_init;
				(*program_repeat_count)=0;
				*await_cycle_count = 0;
			}
		}
		break;


	default:
		*program_state = program_one_fill_init;
	}
}

/////////
//USB related shenanigans
//////////////
uint8_t* greeting = (uint8_t*)"\r\nhello there!\r\n";
uint8_t* followup = (uint8_t*)"\r\nyou are a bold one!\r\n";
uint8_t* longspiel = (uint8_t*)"\r\nyou ever hear the tragedy of darth plagueis the wise?\r\ni thought not. it's not a story the jedi would tell you.\r\nit's a sith legend. darth plagueis was a dark lord of the sith, so powerful and so wise he could use the force to influence the midichlorians to create life...\r\nhe had such a knowledge of the dark side that he could even keep the ones he cared about from dying.\r\nthe dark side of the force is a pathway to many abilities some consider to be unnatural.\r\nhe became so powerful... the only thing he was afraid of was losing his power, which eventually, of course, he did.\r\nunfortunately, he taught his apprentice everything he knew, then his apprentice killed him in his sleep.\r\nit's ironic he could save others from death, but not himself.\r\n";
uint8_t* am_senate = (uint8_t*)"\r\nthe senate!\r\n";
uint8_t* do_it = (uint8_t*)"\r\nit!\r\n";
uint8_t* unlimited_power = (uint8_t*)"\r\npower!\r\n";
uint8_t* hate_sand = (uint8_t*)"\r\nsand! it's coarse and rough and irritating\r\nand it gets everywhere!\r\n";
uint8_t* outrageous_unfair = (uint8_t*)"\r\n! it's unfair!\r\nHow can you be on the Council, and not be a Master?";
uint8_t* what_about = (uint8_t*)"\r\about the droid attack on the wookies?\r\n";
uint8_t* its_a_trap = (uint8_t*)"\r\ntrap!\r\n";
uint8_t* democracy = (uint8_t*)"\r\ndemocracy...\r\n";
uint8_t* you_underestimate = (uint8_t*)"\r\nunderestimate my power!\r\n";
uint8_t* its_over = (uint8_t*)"\r\nanikan i have the high ground!\r\n";
uint8_t* fun_begins = (uint8_t*)"\r\nthe fun begins!\r\n";
uint8_t* fish = (uint8_t*)"\r\nalways a bigger fish.\r\n";
uint8_t* surprise = (uint8_t*)"\r\nto be sure, but a welcome one.\r\n";
uint8_t* fine = (uint8_t*)"\r\naddition to my collection.\r\n";

uint8_t mybuf[1024] = {0};
uint32_t mylen = 0;
//last received cycle time
uint32_t last_cycle_time = 0;
#define MAX_IDLE_CYCLES 10000
void ProcessText() {
	//check to see if we haven't received any text in a while
	if(HAL_GetTick() - last_cycle_time > MAX_IDLE_CYCLES) {
		//if so, send a greeting
		CDC_Transmit_FS(greeting, strlen((char*)greeting));
		last_cycle_time = HAL_GetTick();
		//and clear mybuf
		memset(mybuf, 0, 1024);
		mylen = 0;
	} else //check if mybuf contains the words "general kenobi"
	if(strstr((char*)mybuf, "general kenobi") != NULL) { 
		//if so, send a response
		CDC_Transmit_FS(followup, strlen((char*)followup));
		//and clear mybuf
		memset(mybuf, 0, 1024);
		mylen = 0;
	} else //check if mybuf contains the word "did"
	if(strstr((char*)mybuf, "did") != NULL) {
		CDC_Transmit_FS(longspiel, strlen((char*)longspiel));
		memset(mybuf, 0, 1024);
		mylen = 0;
	} else //check if mybuf contains the word "i am"
	if(strstr((char*)mybuf, "i am") != NULL) {
		CDC_Transmit_FS(am_senate, strlen((char*)am_senate));
		memset(mybuf, 0, 1024);
		mylen = 0;
	} else //check if mybuf contains the word "do"
	if(strstr((char*)mybuf, "do") != NULL) {
		CDC_Transmit_FS(do_it, strlen((char*)do_it));
		memset(mybuf, 0, 1024);
		mylen = 0;
	} else //check if mybuf contains the word "unlimited"
	if(strstr((char*)mybuf, "unlimited") != NULL) {
		CDC_Transmit_FS(unlimited_power, strlen((char*)unlimited_power));
		memset(mybuf, 0, 1024);
		mylen = 0;
	} else //check if mybuf contains the word "i hate"
	if(strstr((char*)mybuf, "i hate") != NULL) {
		CDC_Transmit_FS(hate_sand, strlen((char*)hate_sand));
		memset(mybuf, 0, 1024);
		mylen = 0;
	} else //check if mybuf contains the word "this is outrageous"
	if(strstr((char*)mybuf, "this is") != NULL) {
		CDC_Transmit_FS(outrageous_unfair, strlen((char*)outrageous_unfair));
		memset(mybuf, 0, 1024);
		mylen = 0;
	} else //check if mybuf contains the word "what"
	if(strstr((char*)mybuf, "what") != NULL) {
		CDC_Transmit_FS(what_about, strlen((char*)what_about));
		memset(mybuf, 0, 1024);
		mylen = 0;
	} else //check if mybuf contains the word "it's a"
	if(strstr((char*)mybuf, "it's a") != NULL) {
		CDC_Transmit_FS(its_a_trap, strlen((char*)its_a_trap));
		memset(mybuf, 0, 1024);
		mylen = 0;
	} else //check if mybuf contains the word "i love"
	if(strstr((char*)mybuf, "i love") != NULL) {
		CDC_Transmit_FS(democracy, strlen((char*)democracy));
		memset(mybuf, 0, 1024);
		mylen = 0;
	} else //check if mybuf contains the word "you"
	if(strstr((char*)mybuf, "you") != NULL) {
		CDC_Transmit_FS(you_underestimate, strlen((char*)you_underestimate));
		memset(mybuf, 0, 1024);
		mylen = 0;
	} else //check if mybuf contains the word "it's over"
	if(strstr((char*)mybuf, "it's over") != NULL) {
		CDC_Transmit_FS(its_over, strlen((char*)its_over));
		memset(mybuf, 0, 1024);
		mylen = 0;
	} else //check if mybuf contains the word "this is where"
	if(strstr((char*)mybuf, "this is where") != NULL) {
		CDC_Transmit_FS(fun_begins, strlen((char*)fun_begins));
		memset(mybuf, 0, 1024);
		mylen = 0;
	} else //check if mybuf contains the word "there's"
	if(strstr((char*)mybuf, "there's") != NULL) {
		CDC_Transmit_FS(fish, strlen((char*)fish));
		memset(mybuf, 0, 1024);
		mylen = 0;
	} else //check if mybuf contains the word "a surprise"
	if(strstr((char*)mybuf, "a surprise") != NULL) {
		CDC_Transmit_FS(surprise, strlen((char*)surprise));
		memset(mybuf, 0, 1024);
		mylen = 0;
	} else //check if mybuf contains the word "a fine"
	if(strstr((char*)mybuf, "a fine") != NULL) {
		CDC_Transmit_FS(fine, strlen((char*)fine));
		memset(mybuf, 0, 1024);
		mylen = 0;


	
	
	} else //check if it contains '/r' or '/n'
	if(strstr((char*)mybuf, "\r") != NULL || strstr((char*)mybuf, "\n") != NULL) {
		//if so, clear the contents of mybuf
		memset(mybuf, 0, 1024);
		mylen = 0;
	}
}


void RxText(uint8_t* Buf, uint32_t *Len) {
	last_cycle_time = HAL_GetTick();
	//check if we have room in mybuf
	if(mylen + *Len > 1024) {
		//if not, clear mybuf
		memset(mybuf, 0, 1024);
		mylen = 0;
	}
	for(uint32_t i = 0; i < *Len; i++) {
		mybuf[mylen+i] = Buf[i];
	}
	CDC_Transmit_FS(&mybuf[mylen], *Len);
	mylen += *Len;
}



/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
	__HAL_AFIO_REMAP_SWJ_DISABLE(); //necessary as LEDs are using JTAG pins, comment out if you want debug support

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */


  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    int program_repeat_count = 0;
	enum program_state_e program_state = program_three_init;
	uint8_t led_cycle_count = 0;
	uint32_t cycle_count = 0;
	uint32_t await_cycle_count = 480;

//	int8_t velocity = 1;
//	uint8_t fill = 0;
//	uint8_t startindex = leds_get_random_index();
//	leds_program_one_fill_init(startindex, fill, velocity);
	//make a random number between 0 and 32 for startindex
	while (1)
	{
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		ProcessText();
		if(cycle_count >= await_cycle_count) {
			advance_state(&program_repeat_count, &program_state, cycle_count, &await_cycle_count);
			//leds_status[1].brightness = 1;
			cycle_count = 0;
		}
		cycle_count++;

		led_cycle_count++;
		if(led_cycle_count > LED_BRIGHTNESS_CYCLES)
			led_cycle_count = 1;

		leds_emit(led_cycle_count);


		//CDC_Transmit_FS("hi\n", 3);
		//HAL_Delay(1);
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LED3_Pin|LED2_Pin|LED1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED33_Pin|LED32_Pin|LED31_Pin|LED30_Pin
                          |LED29_Pin|LED28_Pin|LED27_Pin|LED26_Pin
                          |LED16_Pin|LED15_Pin|LED14_Pin|LED13_Pin
                          |LED12_Pin|LED11_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED25_Pin|LED24_Pin|LED23_Pin|LED22_Pin
                          |LED21_Pin|LED20_Pin|LED19_Pin|LED18_Pin
                          |LED17_Pin|LED10_Pin|LED9_Pin|LED8_Pin
                          |LED7_Pin|LED6_Pin|LED5_Pin|LED4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LED3_Pin LED2_Pin LED1_Pin */
  GPIO_InitStruct.Pin = LED3_Pin|LED2_Pin|LED1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LED33_Pin LED32_Pin LED31_Pin LED30_Pin
                           LED29_Pin LED28_Pin LED27_Pin LED26_Pin
                           LED16_Pin LED15_Pin LED14_Pin LED13_Pin
                           LED12_Pin LED11_Pin */
  GPIO_InitStruct.Pin = LED33_Pin|LED32_Pin|LED31_Pin|LED30_Pin
                          |LED29_Pin|LED28_Pin|LED27_Pin|LED26_Pin
                          |LED16_Pin|LED15_Pin|LED14_Pin|LED13_Pin
                          |LED12_Pin|LED11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LED25_Pin LED24_Pin LED23_Pin LED22_Pin
                           LED21_Pin LED20_Pin LED19_Pin LED18_Pin
                           LED17_Pin LED10_Pin LED9_Pin LED8_Pin
                           LED7_Pin LED6_Pin LED5_Pin LED4_Pin */
  GPIO_InitStruct.Pin = LED25_Pin|LED24_Pin|LED23_Pin|LED22_Pin
                          |LED21_Pin|LED20_Pin|LED19_Pin|LED18_Pin
                          |LED17_Pin|LED10_Pin|LED9_Pin|LED8_Pin
                          |LED7_Pin|LED6_Pin|LED5_Pin|LED4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
