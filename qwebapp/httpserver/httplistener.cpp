/**
  @file
  @author Stefan Frings
*/

#include "httplistener.h"
#include "httpconnectionhandler.h"
#include "httpconnectionhandlerpool.h"
#include "utils.h"
#include <QCoreApplication>

using namespace stefanfrings;

HttpListener::HttpListener(HttpRequestHandler* requestHandler, QObject *parent)
    : QTcpServer(parent)
{

    Q_ASSERT(requestHandler!=nullptr);
    pool=nullptr;

    this->requestHandler=requestHandler;
    // Reqister type of socketDescriptor for signal/slot handling
    qRegisterMetaType<tSocketDescriptor>("tSocketDescriptor");
    // Start listening
    listen();
}


HttpListener::~HttpListener()
{
    close();
    qDebug("HttpListener: destroyed");
}


void HttpListener::listen()
{
    if (!pool)
    {
        pool=new HttpConnectionHandlerPool(requestHandler);
    }
    QString host = TextLabUtils::getSetting("listener/host").toString();
    quint16 port = TextLabUtils::getSetting("listener/port").toUInt() & 0xFFFF;
    QTcpServer::listen(host.isEmpty() ? QHostAddress::Any : QHostAddress(host), port);
    if (!isListening())
    {
        qCritical("HttpListener: Cannot bind on port %i: %s",port,qPrintable(errorString()));
    }
    else {
        qInfo()<<"HttpListener: Listening on "<<host<<"port"<<QString::number(port);
    }
}


void HttpListener::close() {
    QTcpServer::close();
    qDebug("HttpListener: closed");
    if (pool) {
        delete pool;
        pool=nullptr;
    }
}

void HttpListener::incomingConnection(tSocketDescriptor socketDescriptor) {
#ifdef SUPERVERBOSE
    qDebug("HttpListener: New connection");
#endif

    QElapsedTimer timeout;
    HttpConnectionHandler* nHandler=nullptr;

    int connectionTimeout  = TextLabUtils::getSetting("listener/connectionTimeout",1000).toInt() ;
    timeout.start();

    while (!nHandler && timeout.elapsed() < connectionTimeout ) {

        if (pool)
        {
            nHandler=pool->getConnectionHandler();
        }
    }

    // Let the handler process the new connection.
    if (nHandler)
    {
        // The descriptor is passed via event queue because the handler lives in another thread
        QMetaObject::invokeMethod(nHandler, "handleConnection", Qt::QueuedConnection, Q_ARG(tSocketDescriptor, socketDescriptor));
    }
    else
    {
        // Reject the connection
        qWarning("HttpListener: Too many incoming connections");
        QTcpSocket* socket=new QTcpSocket(this);
        socket->setSocketDescriptor(socketDescriptor);
        connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
        socket->write("HTTP/1.1 503 too many connections\r\nConnection: close\r\n\r\n503 Too many connections\r\n");
        socket->waitForBytesWritten(1000);
        socket->disconnectFromHost();
    }
}
