
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

#ifndef __KNETPROC_H
#define __KNETPROC_H

#include <QString>

/**
 * A class used to read raw network throughput details from the system.
 *
 * See knetproc.cpp for details of supported operating systems.
 */
class KNetProc {
public:
    /**
     * The number of bytes in a megabyte for the purposes of totalMbIn()
     * and totalMbOut().
     */
    static const float megabyte;

public:
    /**
     * Constructor and destructor.
     *
     * Routine setDevice() must be called before this reader can be of
     * any use.
     */
    KNetProc();
    ~KNetProc();

    /**
     * Sets the network device to be monitored.  The device should be
     * specified by its interface name, such as "eth0" or "ppp0".
     *
     * An initial set of total traffic readings will be taken.
     */
    void setDevice(const QString &interface);

    /**
     * Returns the network device currently being monitored.
     */
    QString getDevice() const;

    /**
     * Takes a fresh set of throughput readings.  The values returned by
     * routines recentBytesIn() and recentBytesOut() refer to the time
     * period between this and the previous reading (or between this
     * reading and the object's construction if there was no previous
     * reading).
     */
    void readLoad();

    /**
     * The total number of bytes that have passed through the selected
     * device between the last reading and the previous reading.
     *
     * The values returned are guaranteed to be non-negative.
     */
    float recentBytesIn() const;
    float recentBytesOut() const;

    /**
     * The total amount of traffic that has passed through the selected
     * device, measured in megabytes.  This amount is measured over the
     * entire history of the device, not just from the last reading.
     *
     * See the const KNetProc::megabyte for exactly what constitutes a
     * megabyte for our purposes.
     */
    float totalMbIn() const;
    float totalMbOut() const;

private:
    /**
     * Device and results of readings.
     */
    QString device;
        /**< The interface name of the device to monitor. */
    float currIn, currOut;
        /**< Total traffic in bytes from the last reading. */
    float prevIn, prevOut;
        /**< Total traffic in bytes from the next-to-last reading. */

    void readLoadLinux24();
        /**< Read load data for Linux until 2.4 */
    void readLoadLinux26();
        /**< Read load data for Linux 2.5 and after */
    void readLoadBSD4();
        /**< Read load data for BSD */
    void readLoadSolaris();
        /**< Read load data for Solaris */

    /**
     * OS-specific data members.
     */

    // ========== BSD-specific (begin) ==========
#ifdef Q_OS_BSD4
    char* buf;
        /**< A temporary buffer whose size may need to change. */
    size_t alloc;
        /**< The number of bytes currently allocated for buf. */
#endif
    // ========== BSD-specific (end) ==========
    
    // ========== Linux specific (begin) ==========
#ifdef Q_OS_LINUX
    bool useSys;
#endif
    // ========== Linux specific (end) ==========
};

inline QString KNetProc::getDevice() const {
    return device;
}

inline float KNetProc::recentBytesIn() const {
    return currIn - prevIn;
}
inline float KNetProc::recentBytesOut() const {
    return currOut - prevOut;
}

inline float KNetProc::totalMbIn() const {
    return currIn / megabyte;
}
inline float KNetProc::totalMbOut() const {
    return currOut / megabyte;
}

#endif
