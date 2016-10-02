
#ifndef MAPLE_CONFIG_H
#define MAPLE_CONFIG_H

//* configuration for tuner/demodulator mode selection.
#define DEMODULATOR_MODE                        (1) //* 0: DVB-T
                                                    //* 1: ISDB-T
                                                    //* 2: CMMB
                                                    //* 3: DTMB
                                                    //* 4: ATSC

//* configuration for interface between TS controller and Demodulator.
#define TS_INTERFACE_TYPE                       (0) //* 0: using ts spi;            1: using ts ssi.
//#define TS_INTERFACE_PACKET_SYNC_METHOD         (1) //* 0: by PSync signal;         1: by 0x47 sync byte.
//#define TS_INTERFACE_SSI_DATA_ORDER             (0) //* 0: MSB first;               1: LSB first.
//#define TS_INTERFACE_CLOCK_SIGNAL_POLARITY      (0) //* 0: rising edge capture;     1: faling edge capture.
//#define TS_INTERFACE_ERROR_SIGNAL_POLARITY      (0) //* 0: high level active;       1: low level active.
//#define TS_INTERFACE_DATA_VALID_SIGNAL_POLARITY (0) //* 0: high level active;       1: low level active.
//#define TS_INTERFACE_PSYNC_SIGNAL_POLARITY      (0) //* 0: high level active;       1: low level active.
//#define TS_INTERFACE_PACKET_SIZE                (0) //* 0: 188;             1: 192;             2: 204.

//* configuration for TV scanning library.
#define MAPLE_TV_STANDAR                        (MAPLE_TV_ISDB_T) //* MAPLE_TV_DVB_T: DVB-T
                                                                  //* MAPLE_TV_ISDB_T: ISDB-T
                                                                  //* MAPLE_TV_CMMB: CMMB
                                                                   //* MAPLE_TV_DTMB: DTMB
                                                                 //* MAPLE_TV_ATSC: ATSC
#endif

