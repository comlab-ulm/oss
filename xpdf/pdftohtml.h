#ifndef PDFTOHTML_H
#define PDFTOHTML_H

#include "config.h"
#include "Error.h"
#include "ErrorCodes.h"
#include "pdftohtml_global.h"
#include <QString>


#include <aconf.h>
#include <stdio.h>
#include <stdlib.h>
#include "goo/gmem.h"
#include "goo/gmempp.h"
#include "goo/parseargs.h"
#include <goo/gfile.h>
#include "goo/GString.h"
#include "GlobalParams.h"
#include "PDFDoc.h"
#include "HTMLGen.h"

#include "config.h"
#include <QtDebug>

class PDFTOHTML_EXPORT Pdftohtml
{
public:
    Pdftohtml();
    QStringList convert(std::string inputFileName, std::string outputFileDir);
    QStringList convert(QByteArray&);
};

#endif // PDFTOHTML_H
