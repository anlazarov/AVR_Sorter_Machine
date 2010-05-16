After 4 different operating systems, I managed to get everything working. Kiki

We updated the repository, so the project files are not included. Now only src
and a few sucky files, just like the one you are reading now.

Working source framework: http://github.com/anlazarov/AVR_Sorter_Machine/commit/d9a1c63fc3a73fbf8f23634941a868b7b6d35fa3

Report:
[2010/05/16 03:09]
Program description:
User puts brick on a BELT_1, sensor #1 detects it and let it go to sensor #2. Sensor #2 reads the color and 
run MOTOR_BUF and MOTOR_2 until the brick is dispatched and MOTOR_SORT sorts it.

If we have more bricks than BUFFER(3), MOTOR_1 stops.

Sorting:
* if sensor #2 detects yellow it sorts one way
* if it detects black it sorts the other way
* if it detects something else (except nothing ;)) it does nothing

The default position of the sorting arm (MOTOR_SORT) is in horizontal position (to be able to sort on sides 
and let unknown colors to go through);

Known software limitations:
 - color detection is not 100%
	|_ * sorting is not working well
	|_ * sometimes we get "ghost" cubes sorted => brick_count decreased

Known LEGO design limitations:
 - bricks can get stuck in the end of BELT_1
 - bricks can sometimes not be able to go through MOTOR_BUF
