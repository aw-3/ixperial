#pragma once

#define ADDRESSPROXY_NONE	-1

class RecvTable;
class RecvProp;

#define MAX_DATATABLES		1024	// must be a power of 2.
#define MAX_DATATABLE_PROPS	4096

#define MAX_ARRAY_ELEMENTS	2048		// a network array should have more that 1024 elements

#define HIGH_DEFAULT		-121121.121121f

#define BITS_FULLRES	-1	// Use the full resolution of the type being encoded.
#define BITS_WORLDCOORD	-2	// Encode as a world coordinate.

#define DT_MAX_STRING_BITS			9
#define DT_MAX_STRING_BUFFERSIZE	(1<<DT_MAX_STRING_BITS)	// Maximum length of a string that can be sent.

#define STRINGBUFSIZE(className, varName)	sizeof( ((className*)0)->varName )

// Gets the size of a variable in a class.
#define PROPSIZEOF(className, varName)		sizeof(((className*)0)->varName)


// SendProp::m_Flags.
#define SPROP_UNSIGNED			(1<<0)	// Unsigned integer data.

#define SPROP_COORD				(1<<1)	// If this is set, the float/vector is treated like a world coordinate.
// Note that the bit count is ignored in this case.

#define SPROP_NOSCALE			(1<<2)	// For floating point, don't scale into range, just take value as is.

#define SPROP_ROUNDDOWN			(1<<3)	// For floating point, limit high value to range minus one bit unit

#define SPROP_ROUNDUP			(1<<4)	// For floating point, limit low value to range minus one bit unit

#define SPROP_NORMAL			(1<<5)	// If this is set, the vector is treated like a normal (only valid for vectors)

#define SPROP_EXCLUDE			(1<<6)	// This is an exclude prop (not excludED, but it points at another prop to be excluded).

#define SPROP_XYZE				(1<<7)	// Use XYZ/Exponent encoding for vectors.

#define SPROP_INSIDEARRAY		(1<<8)	// This tells us that the property is inside an array, so it shouldn't be put into the
// flattened property list. Its array will point at it when it needs to.

#define SPROP_PROXY_ALWAYS_YES	(1<<9)	// Set for datatable props using one of the default datatable proxies like
// SendProxy_DataTableToDataTable that always send the data to all clients.

#define SPROP_CHANGES_OFTEN		(1<<10)	// this is an often changed field, moved to head of sendtable so it gets a small index

#define SPROP_IS_A_VECTOR_ELEM	(1<<11)	// Set automatically if SPROP_VECTORELEM is used.

#define SPROP_COLLAPSIBLE		(1<<12)	// Set automatically if it's a datatable with an offset of 0 that doesn't change the pointer
// (ie: for all automatically-chained base classes).
// In this case, it can get rid of this SendPropDataTable altogether and spare the
// trouble of walking the hierarchy more than necessary.

#define SPROP_COORD_MP					(1<<13) // Like SPROP_COORD, but special handling for multiplayer games
#define SPROP_COORD_MP_LOWPRECISION 	(1<<14) // Like SPROP_COORD, but special handling for multiplayer games where the fractional component only gets a 3 bits instead of 5
#define SPROP_COORD_MP_INTEGRAL			(1<<15) // SPROP_COORD_MP, but coordinates are rounded to integral boundaries

#define SPROP_VARINT					SPROP_NORMAL	// reuse existing flag so we don't break demo. note you want to include SPROP_UNSIGNED if needed, its more efficient

#define SPROP_NUMFLAGBITS_NETWORKED		16

// This is server side only, it's used to mark properties whose SendProxy_* functions encode against gpGlobals->tickcount (the only ones that currently do this are
//  m_flAnimTime and m_flSimulationTime.  MODs shouldn't need to mess with this probably
#define SPROP_ENCODED_AGAINST_TICKCOUNT	(1<<16)

// See SPROP_NUMFLAGBITS_NETWORKED for the ones which are networked
#define SPROP_NUMFLAGBITS				17

// Used by the SendProp and RecvProp functions to disable debug checks on type sizes.
#define SIZEOF_IGNORE		-1


// Use this to extern send and receive datatables, and reference them.
#define EXTERN_SEND_TABLE(tableName)	namespace tableName {extern SendTable g_SendTable;}
#define EXTERN_RECV_TABLE(tableName)	namespace tableName {extern RecvTable g_RecvTable;}

#define REFERENCE_SEND_TABLE(tableName)	tableName::g_SendTable
#define REFERENCE_RECV_TABLE(tableName)	tableName::g_RecvTable


typedef enum
{
	DPT_Int = 0,
	DPT_Float,
	DPT_Vector,
	DPT_VectorXY, // Only encodes the XY of a vector, ignores Z
	DPT_String,
	DPT_Array,	// An array of the base types (can't be of datatables).
	DPT_DataTable,
#if 0 // We can't ship this since it changes the size of DTVariant to be 20 bytes instead of 16 and that breaks MODs!!!
	DPT_Quaternion,
#endif

#ifdef SUPPORTS_INT64
	DPT_Int64,
#endif

	DPT_NUMSendPropTypes

} SendPropType;

class DVariant
{
public:
	DVariant() { m_Type = DPT_Float; }
	DVariant(float val) { m_Type = DPT_Float; m_Float = val; }

	const char *ToString()
	{
		static char text[128];

		switch (m_Type)
		{
		case DPT_Int:
			snprintf(text, sizeof(text), "%i", m_Int);
			break;
		case DPT_Float:
			snprintf(text, sizeof(text), "%.3f", m_Float);
			break;
		case DPT_Vector:
			snprintf(text, sizeof(text), "(%.3f,%.3f,%.3f)",
				m_Vector[0], m_Vector[1], m_Vector[2]);
			break;
		case DPT_VectorXY:
			snprintf(text, sizeof(text), "(%.3f,%.3f)",
				m_Vector[0], m_Vector[1]);
			break;
#if 0 // We can't ship this since it changes the size of DTVariant to be 20 bytes instead of 16 and that breaks MODs!!!
		case DPT_Quaternion:
			Q_snprintf(text, sizeof(text), "(%.3f,%.3f,%.3f %.3f)",
				m_Vector[0], m_Vector[1], m_Vector[2], m_Vector[3]);
			break;
#endif
		case DPT_String:
			if (m_pString)
				return m_pString;
			else
				return "NULL";
			break;
		case DPT_Array:
			snprintf(text, sizeof(text), "Array");
			break;
		case DPT_DataTable:
			snprintf(text, sizeof(text), "DataTable");
			break;
#ifdef SUPPORTS_INT64
		case DPT_Int64:
			snprintf(text, sizeof(text), "%I64d", m_Int64);
			break;
#endif
		default:
			snprintf(text, sizeof(text), "DVariant type %i unknown", m_Type);
			break;
		}

		return text;
	}

	union
	{
		float	m_Float;
		int		m_Int;
		const char	*m_pString;
		void	*m_pData;	// For DataTables.
#if 0 // We can't ship this since it changes the size of DTVariant to be 20 bytes instead of 16 and that breaks MODs!!!
		float	m_Vector[4];
#else
		float	m_Vector[3];
#endif

#ifdef SUPPORTS_INT64
		int64	m_Int64;
#endif
	};
	SendPropType	m_Type;
};

// This is passed into RecvProxy functions.
class CRecvProxyData
{
public:
	const RecvProp	*m_pRecvProp;		// The property it's receiving.

	DVariant		m_Value;			// The value given to you to store.

	int				m_iElement;			// Which array element you're getting.

	int				m_ObjectID;			// The object being referred to.
};


//-----------------------------------------------------------------------------
// pStruct = the base structure of the datatable this variable is in (like C_BaseEntity)
// pOut    = the variable that this this proxy represents (like C_BaseEntity::m_SomeValue).
//
// Convert the network-standard-type value in m_Value into your own format in pStruct/pOut.
//-----------------------------------------------------------------------------
typedef void(*RecvVarProxyFn)(const CRecvProxyData *pData, void *pStruct, void *pOut);

// ------------------------------------------------------------------------ //
// ArrayLengthRecvProxies are optionally used to get the length of the 
// incoming array when it changes.
// ------------------------------------------------------------------------ //
typedef void(*ArrayLengthRecvProxyFn)(void *pStruct, int objectID, int currentArrayLength);


// NOTE: DataTable receive proxies work differently than the other proxies.
// pData points at the object + the recv table's offset.
// pOut should be set to the location of the object to unpack the data table into.
// If the parent object just contains the child object, the default proxy just does *pOut = pData.
// If the parent object points at the child object, you need to dereference the pointer here.
// NOTE: don't ever return null from a DataTable receive proxy function. Bad things will happen.
typedef void(*DataTableRecvVarProxyFn)(const RecvProp *pProp, void **pOut, void *pData, int objectID);


// This is used to fork over the standard proxy functions to the engine so it can
// make some optimizations.
class CStandardRecvProxies
{
public:
	CStandardRecvProxies();

	RecvVarProxyFn m_Int32ToInt8;
	RecvVarProxyFn m_Int32ToInt16;
	RecvVarProxyFn m_Int32ToInt32;
	RecvVarProxyFn m_FloatToFloat;
	RecvVarProxyFn m_VectorToVector;
#ifdef SUPPORTS_INT64
	RecvVarProxyFn m_Int64ToInt64;
#endif
};
extern CStandardRecvProxies g_StandardRecvProxies;


class CRecvDecoder;


class RecvProp
{
	// This info comes from the receive data table.
public:
	RecvProp();

	void					InitArray(int nElements, int elementStride);

	int						GetNumElements() const;
	void					SetNumElements(int nElements);

	int						GetElementStride() const;
	void					SetElementStride(int stride);

	int						GetFlags() const;

	const char*				GetName() const;
	SendPropType			GetType() const;

	RecvTable*				GetDataTable() const;
	void					SetDataTable(RecvTable *pTable);

	RecvVarProxyFn			GetProxyFn() const;
	void					SetProxyFn(RecvVarProxyFn fn);

	DataTableRecvVarProxyFn	GetDataTableProxyFn() const;
	void					SetDataTableProxyFn(DataTableRecvVarProxyFn fn);

	int						GetOffset() const;
	void					SetOffset(int o);

	// Arrays only.
	RecvProp*				GetArrayProp() const;
	void					SetArrayProp(RecvProp *pProp);

	// Arrays only.
	void					SetArrayLengthProxy(ArrayLengthRecvProxyFn proxy);
	ArrayLengthRecvProxyFn	GetArrayLengthProxy() const;

	bool					IsInsideArray() const;
	void					SetInsideArray();

	// Some property types bind more data to the prop in here.
	const void*			GetExtraData() const;
	void				SetExtraData(const void *pData);

	// If it's one of the numbered "000", "001", etc properties in an array, then
	// these can be used to get its array property name for debugging.
	const char*			GetParentArrayPropName();
	void				SetParentArrayPropName(const char *pArrayPropName);

public:

	const char              *m_pVarName;
	SendPropType			m_RecvType;
	int						m_Flags;
	int						m_StringBufferSize;


private:

	bool					m_bInsideArray;		// Set to true by the engine if this property sits inside an array.

												// Extra data that certain special property types bind to the property here.
	const void *m_pExtraData;

	// If this is an array (DPT_Array).
	RecvProp				*m_pArrayProp;
	ArrayLengthRecvProxyFn	m_ArrayLengthProxy;

	RecvVarProxyFn			m_ProxyFn;
	DataTableRecvVarProxyFn	m_DataTableProxyFn;	// For RDT_DataTable.

	RecvTable				*m_pDataTable;		// For RDT_DataTable.
	int						m_Offset;

	int						m_ElementStride;
	int						m_nElements;

	// If it's one of the numbered "000", "001", etc properties in an array, then
	// these can be used to get its array property name for debugging.
	const char				*m_pParentArrayPropName;
};


class RecvTable
{
public:

	typedef RecvProp	PropType;

	RecvTable();
	RecvTable(RecvProp *pProps, int nProps, const char *pNetTableName);
	~RecvTable();

	void		Construct(RecvProp *pProps, int nProps, const char *pNetTableName);

	int			GetNumProps();
	RecvProp*	GetProp(int i);

	const char*	GetName();

	// Used by the engine while initializing array props.
	void		SetInitialized(bool bInitialized);
	bool		IsInitialized() const;

	// Used by the engine.
	void		SetInMainList(bool bInList);
	bool		IsInMainList() const;


public:

	// Properties described in a table.
	RecvProp		*m_pProps;
	int				m_nProps;

	// The decoder. NOTE: this covers each RecvTable AND all its children (ie: its children
	// will have their own decoders that include props for all their children).
	CRecvDecoder	*m_pDecoder;

	const char		*m_pNetTableName;	// The name matched between client and server.


private:

	bool			m_bInitialized;
	bool			m_bInMainList;
};


inline int RecvTable::GetNumProps()
{
	return m_nProps;
}

inline RecvProp* RecvTable::GetProp(int i)
{
	return &m_pProps[i];
}

inline const char* RecvTable::GetName()
{
	return m_pNetTableName;
}

inline void RecvTable::SetInitialized(bool bInitialized)
{
	m_bInitialized = bInitialized;
}

inline bool RecvTable::IsInitialized() const
{
	return m_bInitialized;
}

inline void RecvTable::SetInMainList(bool bInList)
{
	m_bInMainList = bInList;
}

inline bool RecvTable::IsInMainList() const
{
	return m_bInMainList;
}


// ------------------------------------------------------------------------------------------------------ //
// See notes on BEGIN_SEND_TABLE for a description. These macros work similarly.
// ------------------------------------------------------------------------------------------------------ //
#define BEGIN_RECV_TABLE(className, tableName) \
	BEGIN_RECV_TABLE_NOBASE(className, tableName) \
		RecvPropDataTable("baseclass", 0, 0, className::BaseClass::m_pClassRecvTable, DataTableRecvProxy_StaticDataTable),

#define BEGIN_RECV_TABLE_NOBASE(className, tableName) \
	template <typename T> int ClientClassInit(T *); \
	namespace tableName { \
		struct ignored; \
	} \
	template <> int ClientClassInit<tableName::ignored>(tableName::ignored *); \
	namespace tableName {	\
		RecvTable g_RecvTable; \
		int g_RecvTableInit = ClientClassInit((tableName::ignored *)NULL); \
	} \
	template <> int ClientClassInit<tableName::ignored>(tableName::ignored *) \
	{ \
		typedef className currentRecvDTClass; \
		const char *pRecvTableName = #tableName; \
		RecvTable &RecvTable = tableName::g_RecvTable; \
		static RecvProp RecvProps[] = { \
			RecvPropInt("should_never_see_this", 0, sizeof(int)),		// It adds a dummy property at the start so you can define "empty" SendTables.

#define END_RECV_TABLE() \
			}; \
		RecvTable.Construct(RecvProps+1, sizeof(RecvProps) / sizeof(RecvProp) - 1, pRecvTableName); \
		return 1; \
	}


#define RECVINFO(varName)						#varName, offsetof(currentRecvDTClass, varName), sizeof(((currentRecvDTClass*)0)->varName)
#define RECVINFO_NAME(varName, remoteVarName)	#remoteVarName, offsetof(currentRecvDTClass, varName), sizeof(((currentRecvDTClass*)0)->varName)
#define RECVINFO_STRING(varName)				#varName, offsetof(currentRecvDTClass, varName), STRINGBUFSIZE(currentRecvDTClass, varName)
#define RECVINFO_BASECLASS(tableName)			RecvPropDataTable("this", 0, 0, &REFERENCE_RECV_TABLE(tableName))
#define RECVINFO_ARRAY(varName)					#varName, offsetof(currentRecvDTClass, varName), sizeof(((currentRecvDTClass*)0)->varName[0]), sizeof(((currentRecvDTClass*)0)->varName)/sizeof(((currentRecvDTClass*)0)->varName[0])

// Just specify the name and offset. Used for strings and data tables.
#define RECVINFO_NOSIZE(varName)				#varName, offsetof(currentRecvDTClass, varName)
#define RECVINFO_DT(varName)					RECVINFO_NOSIZE(varName)
#define RECVINFO_DTNAME(varName,remoteVarName)	#remoteVarName, offsetof(currentRecvDTClass, varName)


void RecvProxy_FloatToFloat(const CRecvProxyData *pData, void *pStruct, void *pOut);
void RecvProxy_VectorToVector(const CRecvProxyData *pData, void *pStruct, void *pOut);
void RecvProxy_VectorXYToVectorXY(const CRecvProxyData *pData, void *pStruct, void *pOut);
void RecvProxy_QuaternionToQuaternion(const CRecvProxyData *pData, void *pStruct, void *pOut);
void RecvProxy_Int32ToInt8(const CRecvProxyData *pData, void *pStruct, void *pOut);
void RecvProxy_Int32ToInt16(const CRecvProxyData *pData, void *pStruct, void *pOut);
void RecvProxy_StringToString(const CRecvProxyData *pData, void *pStruct, void *pOut);
void RecvProxy_Int32ToInt32(const CRecvProxyData *pData, void *pStruct, void *pOut);
#ifdef SUPPORTS_INT64
void RecvProxy_Int64ToInt64(const CRecvProxyData *pData, void *pStruct, void *pOut);
#endif

// StaticDataTable does *pOut = pData.
void DataTableRecvProxy_StaticDataTable(const RecvProp *pProp, void **pOut, void *pData, int objectID);

// PointerDataTable does *pOut = *((void**)pData)   (ie: pData is a pointer to the object to decode into).
void DataTableRecvProxy_PointerDataTable(const RecvProp *pProp, void **pOut, void *pData, int objectID);


RecvProp RecvPropFloat(
	const char *pVarName,
	int offset,
	int sizeofVar = SIZEOF_IGNORE,	// Handled by RECVINFO macro, but set to SIZEOF_IGNORE if you don't want to bother.
	int flags = 0,
	RecvVarProxyFn varProxy = RecvProxy_FloatToFloat
	);

RecvProp RecvPropVector(
	const char *pVarName,
	int offset,
	int sizeofVar = SIZEOF_IGNORE,	// Handled by RECVINFO macro, but set to SIZEOF_IGNORE if you don't want to bother.
	int flags = 0,
	RecvVarProxyFn varProxy = RecvProxy_VectorToVector
	);

RecvProp RecvPropVectorXY(
	const char *pVarName,
	int offset,
	int sizeofVar = SIZEOF_IGNORE,	// Handled by RECVINFO macro, but set to SIZEOF_IGNORE if you don't want to bother.
	int flags = 0,
	RecvVarProxyFn varProxy = RecvProxy_VectorXYToVectorXY
	);

// This is here so the RecvTable can look more like the SendTable.
#define RecvPropQAngles RecvPropVector

#if 0 // We can't ship this since it changes the size of DTVariant to be 20 bytes instead of 16 and that breaks MODs!!!

RecvProp RecvPropQuaternion(
	const char *pVarName,
	int offset,
	int sizeofVar = SIZEOF_IGNORE,	// Handled by RECVINFO macro, but set to SIZEOF_IGNORE if you don't want to bother.
	int flags = 0,
	RecvVarProxyFn varProxy = RecvProxy_QuaternionToQuaternion
	);
#endif

RecvProp RecvPropInt(
	const char *pVarName,
	int offset,
	int sizeofVar = SIZEOF_IGNORE,	// Handled by RECVINFO macro, but set to SIZEOF_IGNORE if you don't want to bother.
	int flags = 0,
	RecvVarProxyFn varProxy = 0
	);

RecvProp RecvPropString(
	const char *pVarName,
	int offset,
	int bufferSize,
	int flags = 0,
	RecvVarProxyFn varProxy = RecvProxy_StringToString
	);

RecvProp RecvPropDataTable(
	const char *pVarName,
	int offset,
	int flags,
	RecvTable *pTable,
	DataTableRecvVarProxyFn varProxy = DataTableRecvProxy_StaticDataTable
	);

RecvProp RecvPropArray3(
	const char *pVarName,
	int offset,
	int sizeofVar,
	int elements,
	RecvProp pArrayProp,
	DataTableRecvVarProxyFn varProxy = DataTableRecvProxy_StaticDataTable
	);

// Use the macro to let it automatically generate a table name. You shouldn't 
// ever need to reference the table name. If you want to exclude this array, then
// reference the name of the variable in varTemplate.
RecvProp InternalRecvPropArray(
	const int elementCount,
	const int elementStride,
	const char *pName,
	ArrayLengthRecvProxyFn proxy
	);


//
// Use this if you want to completely manage the way the array data is stored.
// You'll need to provide a proxy inside varTemplate that looks for 'iElement'
// to figure out where to store the specified element.
//
#define RecvPropVirtualArray( arrayLengthProxy, maxArrayLength, varTemplate, propertyName ) \
	varTemplate, \
	InternalRecvPropArray( \
		maxArrayLength, \
		0, \
		#propertyName, \
		arrayLengthProxy \
		)


// Use this and pass the array name and it will figure out the count and stride automatically.
#define RecvPropVariableLengthArray( arrayLengthProxy, varTemplate, arrayName )			\
	varTemplate,										\
	InternalRecvPropArray(								\
		sizeof(((currentRecvDTClass*)0)->arrayName) / PROPSIZEOF(currentRecvDTClass, arrayName[0]), \
		PROPSIZEOF(currentRecvDTClass, arrayName[0]),	\
		#arrayName,										\
		arrayLengthProxy								\
		)


// Use this and pass the array name and it will figure out the count and stride automatically.
#define RecvPropArray( varTemplate, arrayName )			\
	RecvPropVariableLengthArray( 0, varTemplate, arrayName )


// Use this one to specify the element count and stride manually.
#define RecvPropArray2( arrayLengthProxy, varTemplate, elementCount, elementStride, arrayName )		\
	varTemplate,																	\
	InternalRecvPropArray( elementCount, elementStride, #arrayName, arrayLengthProxy )


// ---------------------------------------------------------------------------------------- //
// Inlines.
// ---------------------------------------------------------------------------------------- //

inline void RecvProp::InitArray(int nElements, int elementStride)
{
	m_RecvType = DPT_Array;
	m_nElements = nElements;
	m_ElementStride = elementStride;
}

inline int RecvProp::GetNumElements() const
{
	return m_nElements;
}

inline void RecvProp::SetNumElements(int nElements)
{
	m_nElements = nElements;
}

inline int RecvProp::GetElementStride() const
{
	return m_ElementStride;
}

inline void RecvProp::SetElementStride(int stride)
{
	m_ElementStride = stride;
}

inline int RecvProp::GetFlags() const
{
	return m_Flags;
}

inline const char* RecvProp::GetName() const
{
	return m_pVarName;
}

inline SendPropType RecvProp::GetType() const
{
	return m_RecvType;
}

inline RecvTable* RecvProp::GetDataTable() const
{
	return m_pDataTable;
}

inline void RecvProp::SetDataTable(RecvTable *pTable)
{
	m_pDataTable = pTable;
}

inline RecvVarProxyFn RecvProp::GetProxyFn() const
{
	return m_ProxyFn;
}

inline void RecvProp::SetProxyFn(RecvVarProxyFn fn)
{
	m_ProxyFn = fn;
}

inline DataTableRecvVarProxyFn RecvProp::GetDataTableProxyFn() const
{
	return m_DataTableProxyFn;
}

inline void RecvProp::SetDataTableProxyFn(DataTableRecvVarProxyFn fn)
{
	m_DataTableProxyFn = fn;
}

inline int RecvProp::GetOffset() const
{
	return m_Offset;
}

inline void RecvProp::SetOffset(int o)
{
	m_Offset = o;
}

inline RecvProp* RecvProp::GetArrayProp() const
{
	return m_pArrayProp;
}

inline void RecvProp::SetArrayProp(RecvProp *pProp)
{
	m_pArrayProp = pProp;
}

inline void RecvProp::SetArrayLengthProxy(ArrayLengthRecvProxyFn proxy)
{
	m_ArrayLengthProxy = proxy;
}

inline ArrayLengthRecvProxyFn RecvProp::GetArrayLengthProxy() const
{
	return m_ArrayLengthProxy;
}

inline bool RecvProp::IsInsideArray() const
{
	return m_bInsideArray;
}

inline void RecvProp::SetInsideArray()
{
	m_bInsideArray = true;
}

inline const void* RecvProp::GetExtraData() const
{
	return m_pExtraData;
}

inline void RecvProp::SetExtraData(const void *pData)
{
	m_pExtraData = pData;
}

inline const char* RecvProp::GetParentArrayPropName()
{
	return m_pParentArrayPropName;
}

inline void	RecvProp::SetParentArrayPropName(const char *pArrayPropName)
{
	m_pParentArrayPropName = pArrayPropName;
}
