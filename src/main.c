#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rhd.h"

#include "main.h"
#include "mission/mission.h"
#include "mission/task.h"
#include "odometry/odometry.h"
#include "calibration/calibration.h"

input in;
output out;

int
main(int argc, char **argv)
{
    if(argc < 3)
	{
	    printf("Usage: smr <start_state> <speed>\n");
	    return ERROR;
	}

    int result = init();
    if(result)
	{
	    return result;
	}

    //Perform the standard mission given the speed from the command line
    mission(atoi(argv[1]), atoi(argv[2]));

    term(&out);
    return 0;
}

int
init()
{
    if(rhdConnect('w', "localhost", ROBOT_PORT) != 'w')
	{
	    printf("Can't connect to rhd \n");
	    return ERROR;
	}
    printf("Connected to robot \n");

    symTableElement *input_table, *output_table;

    if((input_table = getSymbolTable('r')) == NULL)
	{
	    printf("Can't connect to rhd \n");
	    return ERROR;
	}
    if((output_table = getSymbolTable('w')) == NULL)
	{
	    printf("Can't connect to rhd \n");
	    return ERROR;
	}

    out.encoder_left = getinputref("encl", input_table);
    out.encoder_right = getinputref("encr", input_table);
    out.line_sensor = getinputref("linesensor", input_table);
    out.ir_sensor = getinputref("irsensor", input_table);

    in.speed_left = getoutputref("speedl", output_table);
    in.speed_right = getoutputref("speedr", output_table);
    in.reset_motor_left = getoutputref("resetmotorr", output_table);
    in.reset_motor_right = getoutputref("resetmotorl", output_table);
	
    rhdSync();

	//Initialise and reset the general odometry structure,
	//which will keep the movement throughout all missions.
	//
	reset_odometry(&current_odometry);

	load_calibration();

    return 0;
}

void
term()
{
    task(T_STOP, 0, 0);
    rhdSync();
	save_calibration();
    rhdDisconnect();
}

symTableElement *
getinputref(const char *sym_name, symTableElement * tab)
{
    int i;
    for(i = 0; i < getSymbolTableSize('r'); i++)
	if(strcmp(tab[i].name, sym_name) == 0)
	    return &tab[i];
    return 0;
}

symTableElement *
getoutputref(const char *sym_name, symTableElement * tab)
{
    int i;
    for(i = 0; i < getSymbolTableSize('w'); i++)
	if(strcmp(tab[i].name, sym_name) == 0)
	    return &tab[i];
    return 0;
}
