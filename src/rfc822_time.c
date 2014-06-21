#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    /*
    time_t current_time;
    char* c_time_string;

    // Obtain current time as seconds elapsed since the Epoch.
    current_time = time(NULL);

    if (current_time == ((time_t)-1))
    {
        (void) fprintf(stderr, "Failure to compute the current time.");
        return EXIT_FAILURE;
    }

    // Convert to local time format.
    c_time_string = ctime(&current_time);

    if (c_time_string == NULL)
    {
        (void) fprintf(stderr, "Failure to convert the current time.");
        return EXIT_FAILURE;
    }

    // Print to stdout.
    (void) printf("Current time is %s", c_time_string);
    printf("UTC:   %s", asctime(gmtime(&current_time)));
    printf("local: %s", asctime(localtime(&current_time)));
    */
    // "     %T    is equivalent to ``%H:%M:%S''."
    // char timeString[9];  // space for "HH:MM:SS\0"

    // http://stackoverflow.com/questions/1531055/time-into-string-with-hhmmss-format-c-programming#answer-1531151
    // ---------------------------------------
    // #include <stdio.h>
    // #include <time.h>
    //
    // /* get seconds since the Epoch */
    // time_t secs = time(0);
    //
    // /* convert to localtime */
    // struct tm *local = localtime(&secs);
    // /* and set the string */
    // sprintf(timeString, "%02d:%02d:%02d", local->tm_hour, local->tm_min, local->tm_sec);


    // HTTP-date - RFC 822 referenced via RFC2616
    //const char *http_date_fmt = "%a, %d %b %Y %T %Z";
    const char *http_date_fmt = "%a, %d %b %Y %T GMT";
    time_t t = time(NULL);
    //struct tm *timeinfo;

    // The format is a fixed length of 29 chars and we add one for '\0'
    char buf[30];
    //if (strftime(buf, sizeof buf, http_date_fmt, gmtime(&current_time))) {
    if (strftime(buf, sizeof buf, http_date_fmt, gmtime(&t))) {
        puts(buf);
        printf("%lu\n", strlen(buf));
    } else {
        puts("strftime failed.");
    }

    return EXIT_SUCCESS;
}
