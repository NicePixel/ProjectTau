/* timer.h
 * Timer is a way to calculate time between tau_timer_start() and tau_timer_stop() call
 * Only one timer can be run at any given moment.
 */
#pragma once

void   tau_timer_start(void);
double tau_timer_stop(void);
