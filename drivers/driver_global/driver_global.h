#ifndef __DRIVER_GLOBAL_H
#define __DRIVER_GLOBAL_H

#define OK          0
#define ENULLPTR    1
#define EPARAM      2
#define ENOMEM      3
#define ENOSUPPORT  4
#define EOS         5


#define IF_PTR_IS_NULL_RET_NULLPTR_ERR(ptr) \
    do \
    { \
        if (NULL == ptr) \
        { \
            return -ENULLPTR; \
        } \
    } \
    while(0)

#define IF_CONDITION_TURE_RET_PARAM_ERR(condition) \
    do \
    { \
        if (condition) \
        { \
            return -EPARAM; \
        } \
    } \
    while(0)

#endif