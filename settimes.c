// Example program showing how to set all file times on macOS (Darwin)
// 
// Note that you can set these times programmatically:
//  - create time (CRTIME)
//  - access time (ACCTIME)
//  - modification time (MODTIME)
//  - backup time (BKUPTIME)
//
//  You _cannot_ programmatically change:
//  - (CHGTIME)
//
// Compile with:
//  gcc settimes.c -o settimes
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/attr.h>
#include <time.h>
#include <unistd.h>

void usage(char *argv0)
{
    fprintf(stderr, "usage: %s [-abcm] [-s datetime] -f <path>\n", argv0);
    fprintf(stderr, "\n");
    fprintf(stderr, "  -a             set access time (ACCTIME)\n");
    fprintf(stderr, "  -b             set backup time (BKUPTIME)\n");
    fprintf(stderr, "  -c             set create time (CRTIME)\n");
    fprintf(stderr, "  -f <path>      full path to file you want to set times on\n");
    fprintf(stderr, "  -m             set modification time (MODTIME)\n");
    fprintf(stderr, "  -s <datetime>  set datetime on file to <datetime>, which must be in the format\n");
    fprintf(stderr, "                 \"%%m/%%d/%%y %%H:%%M:%%S %%Z\"\n");
    fprintf(stderr, "                 for example, \"02/14/09 05:04:00 GMT\"\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "example:\n");
    fprintf(stderr, "  %s -abcm -s \"02/14/09 05:04:00 GMT-8\" -f /tmp/test.txt\n", argv0);
    fprintf(stderr, "  would set ACCTIME, BKUPTIME, CRTIME and MODTIME to 02/14/09 05:04 PDT on the file /tmp/test.txt\n");
    fprintf(stderr, "  timezones like EDT do not work because of ambiguitity.  For more info, run \"man strptime\"\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "notes:\n");
    fprintf(stderr, "  to view all times for a file, you can use \"stat -x /tmp/test.txt\"\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "\n");
    exit(1);
}
    
void setfiletimes(const char *path, const struct timespec *ts, int aflag, int bflag, int cflag, int mflag)
{
    int             err;
    struct attrlist attrList;

    memset(&attrList, 0, sizeof(attrList));
    attrList.bitmapcount = ATTR_BIT_MAP_COUNT;
    attrList.reserved = 0;
    attrList.dirattr = 0;
    attrList.fileattr = 0;
    attrList.forkattr = 0;
    attrList.volattr = 0;

    if (bflag) {
        attrList.commonattr = ATTR_CMN_BKUPTIME;
        err = setattrlist(path, &attrList, (void*)ts,
                      sizeof(struct timespec), FSOPT_NOFOLLOW);
        if(err != 0) {
            perror("setattrlist(..., ATTR_CMN_BKUPTIME, ...)");
            exit(1);
        }
    }

    if (cflag) {
        attrList.commonattr = ATTR_CMN_CRTIME;
        err = setattrlist(path, &attrList, (void*)ts,
                      sizeof(struct timespec), FSOPT_NOFOLLOW);
        if(err != 0) {
            perror("setattrlist(..., ATTR_CMD_CRTIME, ...)");
            exit(1);
        }
    }

    if (mflag) {
        attrList.commonattr = ATTR_CMN_MODTIME;
        err = setattrlist(path, &attrList, (void*)ts,
                      sizeof(struct timespec), FSOPT_NOFOLLOW);
        if(err != 0) {
            perror("setattrlist(..., ATTR_CMN_MODTIME, ...)");
            exit(1);
        }
    }

    if (aflag) {
        attrList.commonattr = ATTR_CMN_ACCTIME;
        err = setattrlist(path, &attrList, (void*)ts,
                      sizeof(struct timespec), FSOPT_NOFOLLOW);
        if(err != 0) {
            perror("setattrlist(..., ATTR_CMN_ACCTIME, ...)");
            exit(1);
        }
    }

    // NOTE: cannot modify ctime (ATTR_CMN_CHGTIME) which is the time of the
    //       last file status change.
    //       Any attempts to change 
    /*
     * err = setattrlist(path, &attrList, (void*)ts,
     *              sizeof(struct timespec), FSOPT_NOFOLLOW);
     * if(err != 0) {
     *   perror("setattrlist(..., ATTR_CMN_CHGTIME, ...)");
     *   exit(1);
     * }
     */
}

int main(int argc, char *argv[])
{
    int aflag = 0;
    int bflag = 0;
    int cflag = 0;
    int mflag = 0;
    int sflag = 0;
    char *fvalue = NULL;
    char *svalue = NULL;
    int c;

    while ((c = getopt (argc, argv, "abcf:hms:")) != -1) {
        switch (c) {
            case 'a':
                aflag = 1;
                break;
            case 'b':
                bflag = 1;
                break;
            case 'c':
                cflag = 1;
                break;
            case 'f':
                fvalue = optarg;
                break;
            case 'm':
                mflag = 1;
                break;
            case 's':
                sflag = 1;
                svalue = optarg;
                break;
            case '?':
                if (optopt == 's' || optopt == 'f')
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            case 'h':
            default:
                usage(argv[0]);
                break;

        }
    }

    if (sflag && svalue == NULL) {
        fprintf(stderr, "when using -s option, a date/time string must be must be provided\n");
        usage(argv[0]);
        exit(1);
    }
    if (fvalue == NULL) {
        fprintf(stderr, "a file must be provided after -f argument\n");
        usage(argv[0]);
        exit(1);
    }
//  - create time (CRTIME)
//  - access time (ACCTIME)
//  - modification time (MODTIME)
//  - backup time (BKUPTIME)

    struct timespec ts;
    struct tm t;
    timespec_get(&ts, TIME_UTC); // now

    if (svalue != NULL) {
        if ((strptime(svalue, "%m/%d/%y %H:%M:%S %Z", &t)) == NULL) {
            fprintf(stderr, "could not parse time \"%s\"\n", svalue);
            perror("strptime()");
            exit(1);
        }
        ts.tv_sec = timegm(&t);
    }

    setfiletimes(fvalue, &ts, aflag, bflag, cflag, mflag);
}

