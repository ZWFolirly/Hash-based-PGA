//
// Created by zhang on 2019/6/5.
//

#ifndef HGA_CONFIG_H
#define HGA_CONFIG_H

#ifndef SEED
#define SEED 4
#endif

#ifndef POP_NUM
#define POP_NUM 200
#endif

#ifndef CROSS_RATE
#define CROSS_RATE 0.7
#endif

#ifndef MUTATE_RATE
#define MUTATE_RATE 0.05
#endif

#ifndef MAX_FIT
#define MAX_FIT 1<<30
#endif

#ifndef MAX_GEN
#define MAX_GEN 200000
#endif

#ifndef HASH_SIZE
#define HASH_SIZE 1<<24
#endif

#ifndef THREAD_NUM
#define THREAD_NUM 4
#endif

#endif //HGA_CONFIG_H
