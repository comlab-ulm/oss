#include "pdftohtml.h"
#include <iostream>
#include <sstream>
#include <QtCore>
#include <Stream.h>

Pdftohtml::Pdftohtml()
{
}

static GBool createIndex(char *htmlDir);
static int firstPage = 1;
static int lastPage = 0;
static double zoom = 1;
static int resolution = 150;
static GBool noFonts = gFalse;
static GBool skipInvisible = gFalse;
static GBool allInvisible = gFalse;
static char ownerPassword[33] = "\001";
static char userPassword[33] = "\001";
static GBool quiet = gFalse;
static char cfgFileName[256] = "";
static GBool printVersion = gFalse;
static GBool printHelp = gFalse;

static ArgDesc argDesc[] = {
  {"-f",       argInt,      &firstPage,     0,
   "first page to convert"},
  {"-l",       argInt,      &lastPage,      0,
   "last page to convert"},
  {"-z",       argFP,       &zoom,          0,
   "initial zoom level (1.0 means 72dpi)"},
  {"-r",       argInt,      &resolution,    0,
   "resolution, in DPI (default is 150)"},
  {"-nofonts", argFlag, &noFonts,           0,
   "do not extract embedded fonts"},
  {"-skipinvisible", argFlag, &skipInvisible, 0,
   "do not draw invisible text"},
  {"-allinvisible",  argFlag, &allInvisible,  0,
   "treat all text as invisible"},
  {"-opw",     argString,   ownerPassword,  sizeof(ownerPassword),
   "owner password (for encrypted files)"},
  {"-upw",     argString,   userPassword,   sizeof(userPassword),
   "user password (for encrypted files)"},
  {"-q",       argFlag,     &quiet,         0,
   "don't print any messages or errors"},
  {"-cfg",     argString,   cfgFileName,    sizeof(cfgFileName),
   "configuration file to use in place of .xpdfrc"},
  {"-v",       argFlag,     &printVersion,  0,
   "print copyright and version info"},
  {"-h",       argFlag,     &printHelp,     0,
   "print usage information"},
  {"-help",    argFlag,     &printHelp,     0,
   "print usage information"},
  {"--help",   argFlag,     &printHelp,     0,
   "print usage information"},
  {"-?",       argFlag,     &printHelp,     0,
   "print usage information"},
  {NULL}
};

static int writeToFile(void *file, const char *data, int size) {
  return (int)fwrite(data, 1, size, (FILE *)file);
}


static GBool createIndex(char *htmlDir) {
  GString *htmlFileName;
  FILE *html;
  int pg;

  htmlFileName = GString::format("{0:s}/index.html", htmlDir);
  html = openFile(htmlFileName->getCString(), "w");
  if (!html) {
    error(errIO, -1, "Couldn't open HTML file '{0:t}'", htmlFileName);
    return gFalse;
  }
  delete htmlFileName;
  fprintf(html, "<html>\n");
  fprintf(html, "<body>\n");
  for (pg = firstPage; pg <= lastPage; ++pg) {
    fprintf(html, "<a href=\"page%d.html\">page %d</a><br>\n", pg, pg);
  }
  fprintf(html, "</body>\n");
  fprintf(html, "</html>\n");

  fclose(html);

  return gTrue;
}


QStringList Pdftohtml::convert(std::string inputFileName, std::string outputFileDir)
{
    PDFDoc *doc;
//    char *fileName;
//    char *htmlDir;
    GString *ownerPW, *userPW;
    HTMLGen *htmlGen;
    GString *htmlFileName, *pngFileName, *pngURL;
    FILE *htmlFile, *pngFile;
    int pg, err, exitCode;
    GBool ok;

    QStringList htmlPages;

    exitCode = 99;

    // parse args
//    fixCommandLine(&argc, &argv);
//    ok = parseArgs(argDesc, &argc, argv);
//    if (!ok || argc != 3 || printVersion || printHelp) {
//      fprintf(stderr, "pdftohtml version %s [www.xpdfreader.com]\n", xpdfVersion);
//      fprintf(stderr, "%s\n", xpdfCopyright);
//      if (!printVersion) {
//        printUsage("pdftohtml", "<PDF-file> <html-dir>", argDesc);
//      }
//      goto err0;
//    }

    char *fileName = new char[inputFileName.length() + 1];
    strcpy(fileName, inputFileName.c_str());
    char *htmlDir = new char[outputFileDir.length() + 1];
    strcpy(htmlDir, outputFileDir.c_str());

    // read config file
    quiet = true;
    globalParams = new GlobalParams(cfgFileName);
    if (quiet) {
      globalParams->setErrQuiet(quiet);
    }
    globalParams->setupBaseFonts(NULL);
    globalParams->setTextEncoding("UTF-8");

    // open PDF file
    if (ownerPassword[0] != '\001') {
      ownerPW = new GString(ownerPassword);
    } else {
      ownerPW = NULL;
    }
    if (userPassword[0] != '\001') {
      userPW = new GString(userPassword);
    } else {
      userPW = NULL;
    }
    doc = new PDFDoc(fileName, ownerPW, userPW);
    if (userPW) {
      delete userPW;
    }
    if (ownerPW) {
      delete ownerPW;
    }
    if (!doc->isOk()) {
      exitCode = 1;
      goto err1;
    }

    // check for copy permission
    if (!doc->okToCopy()) {
      error(errNotAllowed, -1,
        "Copying of text from this document is not allowed.");
      exitCode = 3;
      goto err1;
    }

    // get page range
    if (firstPage < 1) {
      firstPage = 1;
    }
    if (lastPage < 1 || lastPage > doc->getNumPages()) {
      lastPage = doc->getNumPages();
    }


    // set up the HTMLGen object
    htmlGen = new HTMLGen(resolution);
    if (!htmlGen->isOk()) {
      exitCode = 99;
      goto err1;
    }
    htmlGen->setZoom(zoom);
    htmlGen->setDrawInvisibleText(!skipInvisible);
    htmlGen->setAllTextInvisible(allInvisible);
    htmlGen->setExtractFontFiles(!noFonts);
    htmlGen->startDoc(doc);

    // convert the pages
    for (pg = firstPage; pg <= lastPage; ++pg) {
      htmlFileName = GString::format("{0:s}/page{1:d}.html", htmlDir, pg);
      pngFileName = GString::format("{0:s}/page{1:d}.png", htmlDir, pg);

      pngURL = GString::format("page{0:d}.png", pg);
      htmlPages.append(QString::fromUtf8(htmlGen->convertPage(pg, pngURL->getCString()
                                 , htmlDir
                                 )));
      delete pngURL;
      delete htmlFileName;
      delete pngFileName;
      if (err != errNone) {
        error(errIO, -1, "Error converting page {0:d}", pg);
        exitCode = 2;
        goto err2;
      }
    }


    exitCode = 0;

    // clean up
   err2:
    delete htmlGen;
   err1:
    delete doc;
    delete globalParams;
   err0:

    // check for memory leaks
    Object::memCheck(stderr);
    gMemReport(stderr);

    //return exitCode;
    return htmlPages;
}

QStringList Pdftohtml::convert(QByteArray& pdfData)
{
    PDFDoc *doc = nullptr;
    GString *ownerPW, *userPW;
    HTMLGen *htmlGen;
    GString *htmlFileName, *pngFileName, *pngURL;
    lastPage = 0; // reset cache
    //FILE *htmlFile, *pngFile;
    int pg = -1, err = 0, exitCode = 99;
    //GBool ok;
    //Object obj;

    QStringList htmlPages;

    exitCode = 99;
    char *htmlDir = qApp->applicationDirPath().toLocal8Bit().data();

//    char *fileName = new char[inputFileName.length() + 1];
//    strcpy(fileName, inputFileName.c_str());
//    char *htmlDir = new char[outputFileDir.length() + 1];
//    strcpy(htmlDir, outputFileDir.c_str());

    // read config file
    quiet = true;
    globalParams = new GlobalParams(cfgFileName);
    if (quiet) {
      globalParams->setErrQuiet(quiet);
    }
    globalParams->setupBaseFonts(NULL);
    globalParams->setTextEncoding("UTF-8");

    // open PDF file
    if (ownerPassword[0] != '\001') {
      ownerPW = new GString(ownerPassword);
    } else {
      ownerPW = NULL;
    }
    if (userPassword[0] != '\001') {
      userPW = new GString(userPassword);
    } else {
      userPW = NULL;
    }

    //auto mem* = new MemStream(pdfData.data(),0,pdfData.length()+1,obj);
    Object obj;
    obj.initNull();


    BaseStream *str = new MemStream(pdfData.data(),0,pdfData.length()+1,&obj);
    doc = new PDFDoc(str, ownerPW, userPW);
    if (userPW) {
      delete userPW;
    }
    if (ownerPW) {
      delete ownerPW;
    }
    if (!doc->isOk()) {
      exitCode = 1;
      goto err1;
    }

    // check for copy permission
    if (!doc->okToCopy(gTrue)) {
      error(errNotAllowed, -1,
        "Copying of text from this document is not allowed.");
      exitCode = 3;
      goto err1;
    }

    // get page range
    if (firstPage < 1) {
      firstPage = 1;
    }
    if (lastPage < 1 || lastPage > doc->getNumPages()) {
      lastPage = doc->getNumPages();
    }

    // set up the HTMLGen object
    htmlGen = new HTMLGen(resolution);
    if (!htmlGen->isOk()) {
      exitCode = 99;
      goto err1;
    }
    htmlGen->setZoom(zoom);
    htmlGen->setDrawInvisibleText(!skipInvisible);
    htmlGen->setAllTextInvisible(allInvisible);
    htmlGen->setExtractFontFiles(!noFonts);
    htmlGen->startDoc(doc);

    // convert the pages
    for (pg = firstPage; pg <= lastPage; ++pg) {
      htmlFileName = GString::format("{0:s}/page{1:d}.html", htmlDir, pg);
      pngFileName = GString::format("{0:s}/page{1:d}.png", htmlDir, pg);

      pngURL = GString::format("page{0:d}.png", pg);
      htmlPages.append(QString::fromUtf8(htmlGen->convertPage(pg, pngURL->getCString(), htmlDir)));
      delete pngURL;
      delete htmlFileName;
      delete pngFileName;
      if (err != errNone) {
        error(errIO, -1, "Error converting page {0:d}", pg);
        exitCode = 2;
        goto err2;
      }
    }

    exitCode = 0;

    // clean up
   err2:
    delete htmlGen;
   err1:
    delete doc;
    delete globalParams;
   err0:

    // check for memory leaks
    Object::memCheck(stderr);
    gMemReport(stderr);

    //return exitCode;
    return htmlPages;
}
