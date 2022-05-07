#pragma once


#ifndef MRD_EXPORT
#define MRD_API __declspec(dllimport)

#else 
#define MRD_API __declspec(dllexport)

#endif