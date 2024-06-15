// Very raw implementation of profiling code chunks - how long will code take
//@ Write properly how to use this, and also finish this:
// unhardcode everything, add draw_debug_info switch(I mean if()), maybe draw to screen instead of cli
void measure_time(double start_time){ // @rename start time?
    start_time -= GetTime();
    static const unsigned LENGTH = 1000;
    static double array_for_average[LENGTH] = {0};
    static unsigned array_index = 0;
    if(array_index > LENGTH-1)
    array_index = 0;
    array_for_average[array_index] = -start_time * 1000000.0f;
    array_index++;
    double average_anim_time = 0;
    for (int i = 0; i < LENGTH; ++i) {
        average_anim_time += array_for_average[i];
    }
    average_anim_time /= (double)LENGTH;
    static double get_time_minus_second = GetTime();
    //if (array_index == 999)
    if(GetTime() - (get_time_minus_second+1) > 0 ) {
        printf("Time passed: %fus \n", average_anim_time);
        //printf("Animation processed in %.10fus \n", -time_of_animation_process * 1000000.0f); //microsec
        get_time_minus_second = GetTime();
    }
}
