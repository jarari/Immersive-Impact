#pragma once

#include <new>
template<typename T>
inline T* FormHeap_Allocate(void) {
	//return reinterpret_cast<T*>(g_formHeap->Allocate(sizeof(T), alignon(T), true));
	return reinterpret_cast<T*>(FormHeap_Allocate(sizeof(T)));
}

#define TES_FORMHEAP_REDEFINE_NEW()											\
	static void * operator new(std::size_t size)							\
	{ return FormHeap_Allocate(size); }										\
	static void * operator new(std::size_t size, const std::nothrow_t &)	\
	{ return FormHeap_Allocate(size); }										\
	static void * operator new(std::size_t size, void * ptr)				\
	{ return ptr; }															\
	static void operator delete(void * ptr)									\
	{ FormHeap_Free(ptr); }													\
	static void operator delete(void * ptr, const std::nothrow_t &)			\
	{ FormHeap_Free(ptr); }													\
	static void operator delete(void *, void *)								\
	{ }