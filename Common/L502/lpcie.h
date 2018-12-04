/***************************************************************************//**
    @file lpcie.h
    ���� �������� ����������� �����, �������� � �������, ������� ��������������
    ���� ����������� ��� ������ �� ����� ��������, ��������������� ���������
    lpcie.

    @warning ��� ����������� �� �������� ������������ � ����� ����������
    � ������� ������� ����������. ��� ������ � ����� L502 �������������
    ������������ ������ �������, ������������ � l502api.h

    @date 23.05.2012
    @author Borisov Alexey <borisov@lcard.ru>
*******************************************************************************/

#ifndef LPCIE_H_
#define LPCIE_H_


#include "stddef.h"

#ifdef _WIN32
    #include "Windows.h"

    #define LPCIE_EXPORT(type) type APIENTRY
#else
    #define LPCIE_EXPORT(type) __attribute__ ((visibility("default"))) type
#endif

#if defined(__STDC__)
# define PREDEF_STANDARD_C_1989
# if defined(__STDC_VERSION__)
#  define PREDEF_STANDARD_C_1990
#  if (__STDC_VERSION__ >= 199409L)
#   define PREDEF_STANDARD_C_1994
#  endif
#  if (__STDC_VERSION__ >= 199901L)
#   define PREDEF_STANDARD_C_1999
#  endif
# endif
#endif


#ifndef STDINT_DEFINED
    /* �������� ����������� ���� stdint.h ��� ������������, �������������� C99,
       ��� GCC � ��� MSVC ������� � ������ 16.0 (Visual Studio 10.0) */
    #if defined  (PREDEF_STANDARD_C_1999) || defined   (  __GNUC__  ) \
        || (defined (_MSC_VER) && (_MSC_VER >= 1600)) \
        || (defined (__BORLANDC__) && (__BORLANDC__ >= 0x560))
        #include "stdint.h"
    #else
        #include "pstdint.h"
    #endif
#endif




#ifdef __cplusplus
extern "C" {
#endif

/** ������������ ������ ������ � ��������� ���������� */
#define LPCIE_DEVNAME_SIZE     32
/** ������������ ������ ��������� ������ */
#define LPCIE_SERIAL_SIZE      32

/** �����, ����������� ������ */
typedef enum
{
    /** ������� ������� ��� */
    LPCIE_DEVINFO_FLAGS_DAC_PRESENT = 0x0001,
    /** ������� ������� ���������������� */
    LPCIE_DEVINFO_FLAGS_GAL_PRESENT = 0x0002,
    /** ������� ������� ����������� ���������� */
    LPCIE_DEVINFO_FLAGS_BF_PRESENT  = 0x0004,
    /** �������, ��� ���������� ��� ������� */
    LPCIE_DEVINFO_FLAGS_DEV_OPENED  = 0x0100
} lpcie_devinfo_flags;

/** �����, ������� ����� �������� � LPCIE_GetDevInfoList() */
typedef enum
{    
    /** �������, ��� ����� ������ ������ ����������, ������� ��� �� ������� */
    LPCIE_GETDEVS_FLAGS_ONLY_NOT_OPENED = 0x1
} t_lpcie_getdevs_flags;


/** ���������, ����������� �� �������� ����������, �� ������� � ��� �����
    ���������� ����� */
typedef struct
{
    char devname[LPCIE_DEVNAME_SIZE]; /** �������� ���������� */
    char serial[LPCIE_SERIAL_SIZE]; /** �������� ����� */
    char res[124]; /** ������ */
    uint32_t flags; /** ����� �� #lpcie_devinfo_flags, ����������� ���������� */
    void* intptr; /** ������������ ��������� �� ��������� � ������������������
                      ����������� �� ����������. ���������� ����������� ���
                      ������ LPCIE_GetDevInfoList() � ������ ���� �����������
                      � ������� LPCIE_FreeDevInfoList() */
} t_lpcie_devinfo;



/***************************************************************************//**
    @brief ��������� ������������������ ���������

    ������� ������� ��� ������������ ���������� � �������, ��������������
    ��������� lpcie � ��� ������� ���������� ���������� ��������� ���������
    �� ����������� ������� (���� � ��� ���� �����) ����������, ������������
    ��� ����������. ��� ���� ��� �� ���������� ������ ��� �������� ���������-
    ��������� ���������� ��� ����������� ������������ �������� ����������.
    ��� ������ ������ ���� ����� ������� � ������� LPCIE_FreeDevInfoList().
    @param[out] list     ������ ��������, ������� ������ ���� ��������� �����������
                         � ������������ �����������. ����� ���� NULL, ���� size=0,
                         � devcnt!=NULL, � ������, ���� ����� �������� ����������
                         ���������.
    @param[in] size      ���������� �������� � ������� list. ������� ��������
                         �������� size �������� � list.
    @param[in] flags     ����� �� #t_lpcie_getdevs_flags.
    @param[out] devcnt   ���� �� NULL, �� � ���� ���������� ������������, �������
                         ����� ������� ��������� - ����� ���� ������ size
    @return              ���� ������ ���� - ��� ������,
                         ���� >= 0, ���������� ����������� �������� � list
                         (����� min(size, devcnt))
 ******************************************************************************/
LPCIE_EXPORT(int32_t) LPCIE_GetDevInfoList(t_lpcie_devinfo* list, uint32_t size, uint32_t flags,
                             uint32_t* devcnt);

/***************************************************************************//**
    @brief ������������ ������, ���������� LPCIE_GetDevInfoList().

    ��� ������� �������� ������� � ����������� � ����������, ������������
    �� ����� � ������� LPCIE_GetDevInfoList(), ������ ������� ������������
    ������ � ���������-��������� �����������, ��������� �� ������� ���
    �������� � ���� intptr ������� �������� �������.

    ����� ����� ������ ��������� ��� �� ����� ���� ������������ ��� ��������
    ����������.

    ������� ������ ���������� ������ ���, ����� ���������� � �������
    LPCIE_GetDevInfoList() ���������� ������ �� ���������.

    @note ���� ��� ������ list ��������� �����������, �� ��� ����� ����������
    �������.

    @param[in] list   ������ �������� � ����������� � ������������ �����������
    @param[in] size   ���������� ��������� � �������
    @return           ��� ������
    ***************************************************************************/
LPCIE_EXPORT(int32_t) LPCIE_FreeDevInfoList(t_lpcie_devinfo *list, uint32_t size);



#ifdef __cplusplus
}
#endif


#endif
