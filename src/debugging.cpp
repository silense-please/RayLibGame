// Very raw implementation of profiling the code chunks - how long will code take
//@ Write properly how to use this, and also finish this:
// unhardcode everything, add draw_debug_info switch(I mean if())
// GetTime() doesn't stop counting on a breakpoint program pause
// Make it draw frametime graph later (like fpsMonitor)
// How long is this function takes to execute?? - ~100 assembly inctructions, not clear where is GetTime

//Clock resolution ~ <= 0.2us (microsecond)
//Use of func
//measure_time_start = GetTime(); <- Start measuring
//  ... Some code here ...
//measure_time(measure_time_start); <- End measuring (average time returned)
double measure_time(double start_time){ // @rename: measure average time-finish?
    start_time -= GetTime();
    static const unsigned LENGTH = 1000;
    static double array_for_average[LENGTH] = {0};
    static unsigned array_index = 0;
    if(array_index > LENGTH-1)
        array_index = 0;
    array_for_average[array_index] = -start_time * 1000000.0f; //sec to microsec
    array_index++;
    double average_measured_time = 0;
    for (int i = 0; i < LENGTH; ++i) {
        average_measured_time += array_for_average[i];
    }
    average_measured_time /= (double)LENGTH;
    static double result = average_measured_time;

    // Output(update) every second
    static double get_time_minus_second = GetTime();
    if(GetTime() - (get_time_minus_second+1) > 0 ) {
        //printf("Time passed: %fus \n", average_measured_time);
        result = average_measured_time;

        get_time_minus_second = GetTime();
    }
    return result;
}
