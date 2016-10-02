#ifndef INTERFACE_LINUX_I2C_H
#define INTERFACE_LINUX_I2C_H

#include <adapter/busdescriptor.h>

#ifdef __cplusplus
extern "C" {
#endif

struct dibDataBusHost * host_i2c_interface_attach(void *priv);
void host_i2c_release(struct dibDataBusHost *);

#ifdef __cplusplus
}
#endif

#endif
