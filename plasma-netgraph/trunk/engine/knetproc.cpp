
/***************************************************************************
 *                                                                         *
 *   KNetLoad is copyright (c) 1999-2000, Markus Gustavsson                *
 *                         (c) 2002, Ben Burton                            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "knetproc.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

// BSD-specific includes.
#ifdef Q_OS_BSD4
#include <sys/param.h>
#include <sys/sysctl.h>
#include <net/if_dl.h>
#include <net/route.h>
#endif

// Solaris-specific includes.
#ifdef Q_OS_SOLARIS
#include <kstat.h>
#include <sys/sockio.h>
#endif

// Linux-specific includes.
#ifdef Q_OS_LINUX
#include <QDir>
#endif

/**
 * Linux
 * -----
 *
 * We assume /proc/net/dev is filled with lines of the following form:
 *
 *   iface: bytesIn x x x x x x x bytesOut x x x x x x x
 *
 * We also assume that the first two lines of /proc/net/dev are headers
 * that can be ignored.
 *
 * BSD
 * ---
 *
 * BSD code by Roland Riegel <feedback@roland-riegel.de>,
 * Bill Wilson <bill@gkrellm.net>, Hajimu Umemoto <ume@mahoroba.org> and
 * Anthony Mallet <anthony.mallet@useless-ficus.net>.
 *
 * Solaris
 * -------
 *
 * Solaris code by Roland Riegel <feedback@roland-riegel.de>,
 * Bill Wilson <bill@gkrellm.net> and Daisuke Yabuki <dxy@acm.org>.
 *
 * Other Operating Systems
 * -----------------------
 *
 * Please, send in a patch!
 *
 * The KNetLoad maintainer is currently Ben Burton <bab@debian.org>, or
 * you could submit a patch through the KDE bug tracking system
 * (bugs.kde.org).
 */

#define MAX_NET_DEV_LINE 512
#define MAX_INTERFACE_NAME 128

const float KNetProc::megabyte = 1000000;

KNetProc::KNetProc() {
    currIn = currOut = 0;
    prevIn = prevOut = 0;

    // OS-specific initialisation.

    // ========== BSD-specific (begin) ==========
#ifdef Q_OS_BSD4
    buf = 0;
    alloc = 0;
#endif
    // ========== BSD-specific (end) ==========
    
    // ========== Linux-specific (begin) ==========
#ifdef Q_OS_LINUX
    useSys = QDir::home().exists("/sys/class/net");
#endif
    // ========== Linux-specific (end) ==========
}

KNetProc::~KNetProc() {
    // OS-specific cleaning up.

    // ========== BSD-specific (begin) ==========
#ifdef Q_OS_BSD4
    if (buf)
        free(buf);
#endif
    // ========== BSD-specific (end) ==========
}

void KNetProc::setDevice(const QString &interface) {
    // Actually set the device.
    device = interface;

    // Take an initial reading.
    currIn = currOut = 0;
    prevIn = prevOut = 0;
    readLoad();

    // Make the previous reading identical to the current reading to
    // avoid a massive spike before the next reading is taken.
    prevIn = currIn;
    prevOut = currOut;
}

void KNetProc::readLoad() {
    // The current readings must now become the previous readings.
    prevIn = currIn;
    prevOut = currOut;

    // Take the new set of readings (i.e., update currIn and currOut).

    // If no new readings are taken (e.g., the device could not be
    // found), this code may return; otherwise it *must* fall through to
    // the tidy-up block that follows this OS-specific code.

    // OS-specific local variables.
    
#ifdef Q_OS_LINUX
    if ( useSys )
        readLoadLinux26();
    else
        readLoadLinux24();
#elif defined Q_OS_BSD4
    readLoadBSD4();
#elif defined Q_OS_SOLARIS
    readLoadSolaris();
#endif

    // Tidy up.
    if (currIn < prevIn)
        prevIn = 0;
    if (currOut < prevOut)
        prevIn = 0;
}

// Linux specific function
void KNetProc::readLoadLinux26() {
#ifdef Q_OS_LINUX
    static FILE* fd;
    
    static unsigned int tmp;
    static char *fn;
    asprintf(&fn, "/sys/class/net/%s/statistics/rx_bytes", device.toLatin1().data());
    
    if ( (fd = fopen(fn, "r")) == 0 )
        return;

    fscanf(fd, "%u", &tmp);
    currIn = tmp;

    fclose(fd);
    fn[strlen(fn)-8] = 't'; // "/sys/class/net/%s/statistics/tx_bytes"
    
    if ( (fd = fopen(fn, "r")) == 0 )
        return;

    fscanf(fd, "%u", &tmp);
    currOut = tmp;
    
    fclose(fd);
    free(fn);
#endif
}

void KNetProc::readLoadLinux24() {
#ifdef Q_OS_LINUX
    static FILE* fd;
    static char line[MAX_NET_DEV_LINE];
    static char* pos;
    static char* iface;
    static float junk;
    
    if ((fd = fopen("/proc/net/dev", "r")) == 0)
        return;

    // Read the unwanted header lines.
    fgets(line, MAX_NET_DEV_LINE, fd);
    fgets(line, MAX_NET_DEV_LINE, fd);

    // Read through the remaining lines until we find our device.
    while (! feof(fd)) {
        fgets(line, MAX_NET_DEV_LINE, fd);

        // Find the interface name for this line.
        for (iface = line; *iface == ' '; iface++)
            ; // (skip initial spaces)
        for (pos = iface; *pos != ':' && *pos != 0; pos++)
            ; // (move to next ':' or end of string)
        if (*pos == 0)
            continue; // (was not ':')
        *pos = 0;

        // Now iface points to a null-terminated string containing the
        // interface name for this particular line.
        if (device != iface)
            continue;

        // This is the correct device!
        // Read off the numbers of bytes.
        sscanf(pos + 1, "%f %f %f %f %f %f %f %f %f", &currIn, &junk,
            &junk, &junk, &junk, &junk, &junk, &junk, &currOut);

        // And we're done.
        break;
    }
    // Note that we might have finished the loop without ever finding
    // our device.
    fclose(fd);
#endif
}

// Solaris specific
void KNetProc::readLoadBSD4() {
#ifdef Q_OS_BSD4
    static struct if_msghdr *ifm, *nextifm;
    static struct sockaddr_dl *sdl;
    static char *lim, *next;
    static size_t needed;
    static char s[32];
    static int mib[] = { CTL_NET, PF_ROUTE, 0, 0, NET_RT_IFLIST, 0 };
    if (sysctl(mib, 6, NULL, &needed, NULL, 0) < 0)
        return;
    if (alloc < needed) {
        if (buf != NULL)
            free(buf);
        buf = (char *) malloc(needed);
        if (buf == NULL)
            return;
        alloc = needed;
    }

    if (sysctl(mib, 6, buf, &needed, NULL, 0) < 0)
        return;
    lim = buf + needed;

    next = buf;
    while (next < lim) {
        ifm = (struct if_msghdr *) next;
        if (ifm->ifm_type != RTM_IFINFO)
            return;
        next += ifm->ifm_msglen;

        while (next < lim) {
            nextifm = (struct if_msghdr *) next;
            if (nextifm->ifm_type != RTM_NEWADDR)
                break;
            next += nextifm->ifm_msglen;
        }

        if (ifm->ifm_flags & IFF_UP) {
            sdl = (struct sockaddr_dl *) (ifm + 1);
            if (sdl->sdl_family != AF_LINK)
                continue;
            strncpy(s, sdl->sdl_data, sdl->sdl_nlen);
            s[sdl->sdl_nlen] = 0;

            if (device == s) {
                currIn = ifm->ifm_data.ifi_ibytes;
                currOut = ifm->ifm_data.ifi_obytes;
                break;
            }
        }
    }
#endif
}

// BSD specific code
void KNetProc::readLoadSolaris() {
#ifdef Q_OS_SOLARIS
    static kstat_ctl_t *kc;
    static kstat_t *ksp;
    static kstat_named_t *knp;
    kc = kstat_open();
    ksp = kstat_lookup(kc, 0, -1, device);
    if (ksp && kstat_read(kc, ksp, 0) >= 0) {
        if ((knp = (kstat_named_t *) kstat_data_lookup(ksp, "rbytes")))
            currIn = knp->value.ui32;
        if ((knp = (kstat_named_t *) kstat_data_lookup(ksp, "obytes")))
            currOut = knp->value.ui32;
    }
    kstat_close(kc);
#endif
}
