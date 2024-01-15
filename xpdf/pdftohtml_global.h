#ifndef PDFTOHTML_GLOBAL_H
#define PDFTOHTML_GLOBAL_H

#if defined(PDFTOHTML_LIBRARY)
#  define PDFTOHTML_EXPORT __attribute__((visibility("default")))
#else
#  define PDFTOHTML_EXPORT __attribute__((visibility("default")))
#endif

#endif
