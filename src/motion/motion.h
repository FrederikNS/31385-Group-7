#ifndef _motion_h
#define _motion_h

#include "../mission/task.h"
#include "../calibration/calibration.h"

#define GO_STRAIGHT 0
#define GO_LEFT -1
#define GO_RIGHT 1

void forward(int speed, double expected_distance, task_data_t * task_data);
double speed_calc(double max_speed, double current_distance, double expected_distance);
void follow_line(int direction, double max_speed, int line_color, struct calibration * calibration);

#endif
