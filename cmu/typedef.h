#ifndef _CMU_TYPEDEF_H_
#define _CMU_TYPEDEF_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    typedef unsigned int uint32;
    typedef unsigned short uint16;
    typedef unsigned char uint8;

    typedef int int32;
    typedef short int16;
    typedef signed char int8;

#define DEV_OK (0)
#define DEV_ERROR (-1)
#define DEV_NULLPTR (-2)
#define DEV_PARAMERR (-3)

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !_CMU_TYPEDEF_H_