#ifndef DIBX09X_COMMON_H
#define DIBX09X_COMMON_H

#include <tuner/dib0090.h>
#include <sip/dibx09x.h>

extern const struct dib0090_io_config dibx09x_io_12mhz_120;
extern const struct dib0090_io_config dibx09x_io_26mhz_120;
extern const struct dib0090_io_config dibx09x_io_30mhz_120;

extern const struct dib0090_io_config dibx09x_io_12mhz;
extern const struct dib0090_io_config dibx09x_io_26mhz;
extern const struct dib0090_io_config dibx09x_io_30mhz;

extern struct dibFrontend * dib29000_remote_fe_host_register(struct dibFrontend *fe, int id);
extern void ProcessSlaveSpalMsg(uint32_t * msg);

#endif
