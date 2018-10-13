/*
* Wrappers to emulate dlopen() on other systems like Windows
*/

#pragma once

#if ETH_ETHASHCL
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS true
#define CL_HPP_ENABLE_EXCEPTIONS true
#define CL_HPP_CL_1_2_DEFAULT_BUILD true
#define CL_HPP_TARGET_OPENCL_VERSION 120
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#include <libethash-cl/CL/cl2.hpp>

#ifndef CL_DEVICE_TOPOLOGY_AMD
#define CL_DEVICE_TOPOLOGY_AMD 0x4037
#endif

#ifndef CL_DEVICE_TOPOLOGY_TYPE_PCIE_AMD
#define CL_DEVICE_TOPOLOGY_TYPE_PCIE_AMD 1
#endif

#ifndef CL_DEVICE_PCI_BUS_ID_NV
#define CL_DEVICE_PCI_BUS_ID_NV  0x4008
#endif

#ifndef CL_DEVICE_PCI_SLOT_ID_NV
#define CL_DEVICE_PCI_SLOT_ID_NV 0x4009
#endif

typedef union
{
	struct { cl_uint type; cl_uint data[5]; } raw;
	struct { cl_uint type; cl_char unused[17]; cl_char bus; cl_char device; cl_char function; } pcie;
} cl_device_topology_amd;

#endif

#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)
#include <windows.h>
void *wrap_dlopen(const char *filename);
void *wrap_dlsym(void *h, const char *sym);
int wrap_dlclose(void *h);
#else
/* assume we can use dlopen itself... */
#include <dlfcn.h>
void *wrap_dlopen(const char *filename);
void *wrap_dlsym(void *h, const char *sym);
int wrap_dlclose(void *h);
#endif
