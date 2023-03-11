//----------------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2011.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         EDP300
 Module         HART
 Description    Definition of class T_DATA_OBJ::MAP
 <pre>
    T_DATA_OBJ::MAP extend floating-point objects with an integer-interface.

    Example:

    The float foo is a value in percent shoold be accessed as TINT16 100% = 10000

    The discriptor consits of

    - the reference to the float-object (subsystem index, object index and attribute index
    - the type of the integer (ST_U8, ST_U16 or ST_I16 )
    - gain and offset for linear transformation of the value(s)

    The support of T_DATA_OBJ::MAP arrays is limited to arrays with only 2 members.
    (Because the buffer (*value) is not big enough to hold the floating-point value(s) the Put and the Get methode
     must create an own buffer onto the stack.
     Actual only obects with 2 values must be supported, so to minimize the stack usage only 2 will be supported.)

     Objects with more than MAP_MAX_ARRAY_LENGTH values generates an assert.

     U8, U16 and I16 are supported.


     +------------------------+      int -> flt :  flt = int;  flt = flt*gain+offset
     |   T_DATA_OBJ_MAP:obj1  |      flt -> int :  flt= (flt-offset)/gain; int=(flt>0)?flt+0.5:flt-0.5;
     +------------------------+
     | ptrValue=NIL           |
     | type=U8, U16 or I16    |
     |      ARRAY or SIMPLE   |
     | allways RAM            |      +-----------------------+
     | ptrDescriptor --------------->| T_MAP_DESCRIPTOR:des1 |
     |                        |      +-----------------------+
     +------------------------+      | subsystemIndex \      |        +------------------+
                                     | objectIndex     >------------->| T_DATA_OBJ:obj2  |
                                     | attributeIndex /      |        +------------------+
                                     | gain                  |        | must be ST_FLOAT |
                                     | offset                |        +------------------+
                                     +-----------------------+


 </pre>
 Remarks

*/
//----------------------------------------------------------------------------------------------------------
#ifndef __SYSTEM_H__
  #error system.h must be included before t_data_obj_map.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before t_data_obj_map.h
#endif

#ifndef __T_DATA_OBJ_H__
  #error t_data_obj.h must be included before t_data_obj_map.h
#endif

#if (T_DATA_OBJ_VERSION<0x020101)
  #error T_DATA_OBJ_VERSION must be greater or equal than 2.1.1
#endif

#ifdef __T_DATA_OBJ_MAP_H__
  #error t_data_obj_map.h included more than once
#endif

#define __T_DATA_OBJ_MAP_H__

#define MAP_MAX_ARRAY_LENGTH 2

//----------------------------------------------------------------------------------------------------------
//! initializer for ojects of class T_DATA_OBJ::MAP
extern const T_DATA_CLASS cgClassInitializerST_U8_MAP;
extern const T_DATA_CLASS cgClassInitializerST_U16_MAP;
extern const T_DATA_CLASS cgClassInitializerST_I16_MAP;

//----------------------------------------------------------------------------------------------------------
//! typedef for the T_DATA_OBJ_MAP descriptor
typedef struct _T_MAP_DESCRIPTOR
{
    TUSIGN16 subsystemIndex;
    TUSIGN16 objectIndex;
    TINT16   attributeIndex;
    TFLOAT   gain;
    TFLOAT   offset;
} T_MAP_DESCRIPTOR;




//--------------------------------------------------------------------------------------------------
/*!
 \brief  constructor; Initializer for objects of type simple T_DATA_OBJ::MAP
 \author Heiko Kresse
 \date   2007-04-25
 \param  x simple type ST_U8, ST_U16 and ST_I16 are allowed, other types results in an assert
 \param  z descriptor address
*/
//--------------------------------------------------------------------------------------------------
#define CONSTRUCT_MAP(x,z) { (void FAST*)0,1,SIZE_##x,RAM,SIMPLE, \
                                  (void SLOW const*)(z), &cgClassInitializer##x##_MAP }

//--------------------------------------------------------------------------------------------------
/*!
 \brief  constructor; Initializer for objects of type simple T_DATA_OBJ::MAP
 \author Heiko Kresse
 \date   2007-04-25
 \param  z descriptor address
*/
//--------------------------------------------------------------------------------------------------
#define CONSTRUCT_MAP_U8(z)    CONSTRUCT_MAP(ST_U8,z)
#define CONSTRUCT_MAP_U16(z)   CONSTRUCT_MAP(ST_U16,z)
#define CONSTRUCT_MAP_I16(z)   CONSTRUCT_MAP(ST_I16,z)


//--------------------------------------------------------------------------------------------------
/*!
 \brief  constructor; Initializer for objects of type array T_DATA_OBJ::MAP
 \author Heiko Kresse
 \date   2007-04-25
 \param  x simple type like ST_U8, ST_I16 ....
 \param  y array-length; max 2 otherwise an assert will cause an error
 \param  v descriptor address
*/
//--------------------------------------------------------------------------------------------------
#define CONSTRUCT_ARRAY_MAP(x,y,v) { (void FAST*)0,y,(SIZE_##x)*(y),RAM,ARRAY, \
                                     (void SLOW const*)(v), &cgClassInitializer##x##_MAP }
//--------------------------------------------------------------------------------------------------
/*!
 \brief  constructor; Initializer for objects of type array T_DATA_OBJ::SIMPLE
 \author Heiko Kresse
 \date   2007-04-25
 \param  y array-length
 \param  v descriptor address
*/
//--------------------------------------------------------------------------------------------------
#define CONSTRUCT_ARRAY_MAP_U8(y,v)    CONSTRUCT_ARRAY_MAP(ST_U8,y,v)
#define CONSTRUCT_ARRAY_MAP_U16(y,v)   CONSTRUCT_ARRAY_MAP(ST_U16,y,v)
#define CONSTRUCT_ARRAY_MAP_I16(y,v)   CONSTRUCT_ARRAY_MAP(ST_I16,y,v)



//----------------------------------------------------------------------------------------------------------
//! prototypes of the method implementations of T_DATA_OBJ::SIMPLE

//--------------------------------------------------------------------------------------------------
/*!
 \brief  get the actual object or attribute value.
         Access to memory protected with a resource semaphore will be supported if the pointer to this
         semaphore is not zero.
         Objectes resided in ROM are not supported.

 <pre>
         1. Get data (float or array of float)
         2. transform (array of) U8,U16 or I16
         3. return
 </pre>

 \author Heiko Kresse
 \date   2007-04-25
 \param  the new object or attribute value will be returned in ptrValue
 \param  attributeIndex ==-1 --> Object; >=0 --> Attribute
 \param  ptrDataSemaphore pointer to the resource-semaphore of the used data-memory; 0-ptr-->not protected
 \return error-code
 <pre>
     OK                    operation was successful
     LESSTHAN_RANGEMIN     at least one value is less than its minimum value
     GREATERTHAN_RANGEMAX  at least one value is greater than its maximum
     SPAN_TO_SMALL         then span between two values is to small
     NOT_ON_GRID           at least one value is not a multiple of its increment
     ILLEGAL_ATTRIB_IDX    DEBUG, unknown Attribute
     FATAL_ERROR           object outside nv-mem range
 </pre>
 \warning
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 Get_MAP(const T_DATA_OBJ SLOW*me,
                 void FAST* ptrValue,
                 TINT16 attributeIndex,
                 OS_RSEMA FAST* ptrDataSemaphore);

//--------------------------------------------------------------------------------------------------
/*!
 \brief  put a new object or attribute value into ram, if no businessrules are voilated
         Access to memory protected with a resource semaphore will be supported if the pointer to this
         semaphore is not zero.
 <pre>
         1. transform (arry of) U8, U16 or I16 to float
         2. put data
 </pre>

 \author Heiko Kresse
 \date   2007-04-25
 \param  ptrValue the new object or attribute value; if return!=OK than return the
         nearest possible value in ptrValue
 \param  attributeIndex <0 --> Object; >=0 --> Attribute
 \param  ptrDataSemaphore pointer to the resource-semaphore of the used data-memory; 0-ptr-->not protected
 \warning
 \bug
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 Put_MAP(const T_DATA_OBJ SLOW*me,
                     void FAST* ptrValue,
                     TINT16 attributeIndex,
                     OS_RSEMA FAST* ptrDataSemaphore);


TUSIGN16 GetAttributeDescription_MAP(const T_DATA_OBJ SLOW*me,
                                     T_DO_ATTRIB_RANGE FAST* ptrDescriptor,
                                     TUSIGN16 attributeIndex);



//#define TEST_T_MAP

#ifdef TEST_T_MAP
void TestMAP(void);
#endif
