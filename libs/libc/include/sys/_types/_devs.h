#pragma once

#define MINORBITS 20
#define MINORMASK ((1U << MINORBITS) - 1)
#define major(dev) ((unsigned int)((dev) >> MINORBITS))
#define minor(dev) ((unsigned int)((dev)&MINORMASK))
#define makedev(ma, mi) (((ma) << MINORBITS) | (mi))