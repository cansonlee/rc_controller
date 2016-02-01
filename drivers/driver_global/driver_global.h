#ifndef __DRIVER_GLOBAL_H
#define __DRIVER_GLOBAL_H



#define IF_PTR_IS_NULL_RET_NULLPTR_ERR(ptr) \
		    do \
		    { \
		        if (NULL == ptr) \
		        { \
		            return -1; \
		        } \
		    } \
		    while(0)

#define IF_CONDITION_TURE_RET_PARAM_ERR(condition) \
		    do \
		    { \
		        if (condition) \
		        { \
		            return -1; \
		        } \
		    } \
		    while(0)	


#endif