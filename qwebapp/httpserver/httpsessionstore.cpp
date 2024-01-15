/**
  @file
  @author Stefan Frings
*/

#include "httpsessionstore.h"
#include "utils.h"
#include <QDateTime>
#include <QUuid>

using namespace stefanfrings;

HttpSessionStore::HttpSessionStore(QObject* parent)
    :QObject(parent)
{

    connect(&cleanupTimer,SIGNAL(timeout()),this,SLOT(sessionTimerEvent()));
    cleanupTimer.start(60000);
    cookieName=TextLabUtils::getSetting("listener/cookieName","sessionid").toByteArray();
    expirationTime=TextLabUtils::getSetting("listener/expirationTime",3600000).toInt();
    qDebug("HttpSessionStore: Sessions expire after %i milliseconds",expirationTime);
}

HttpSessionStore::~HttpSessionStore()
{
    cleanupTimer.stop();
}

QByteArray HttpSessionStore::getSessionId(HttpRequest& request, HttpResponse& response)
{
    QMutexLocker locker(&mutex);
    // The session ID in the response has priority because this one will be used in the next request.
    // Get the session ID from the response cookie
    QByteArray sessionId=response.getCookies().value(cookieName).getValue();
    if (sessionId.isEmpty())
    {
        // Get the session ID from the request cookie
        sessionId=request.getCookie(cookieName);
    }
    // Clear the session ID if there is no such session in the storage.
    if (!sessionId.isEmpty())
    {
        if (!sessions.contains(sessionId))
        {
            qDebug("HttpSessionStore: received invalid session cookie with ID %s",sessionId.data());
            sessionId.clear();
        }
    }
    return sessionId;
}

HttpSession HttpSessionStore::getSession(HttpRequest& request, HttpResponse& response, bool allowCreate)
{
    QByteArray sessionId=getSessionId(request,response);

    QMutexLocker locker(&mutex);
    if (!sessionId.isEmpty())
    {
        HttpSession session=sessions.value(sessionId);
        if (!session.isNull())
        {
            // Refresh the session cookie
            QByteArray cookieName=TextLabUtils::getSetting("listener/cookieName","sessionid").toByteArray();
            QByteArray cookiePath=TextLabUtils::getSetting("listener/cookiePath").toByteArray();
            QByteArray cookieComment=TextLabUtils::getSetting("listener/cookieComment").toByteArray();
            QByteArray cookieDomain=TextLabUtils::getSetting("listener/cookieDomain").toByteArray();
            response.setCookie(HttpCookie(cookieName,session.getId(),expirationTime/1000,cookiePath,cookieComment,cookieDomain));
            session.setLastAccess();
            return session;
        }
    }
    // Need to create a new session
    if (allowCreate)
    {
        QByteArray cookieName=TextLabUtils::getSetting("listener/cookieName","sessionid").toByteArray();
        QByteArray cookiePath=TextLabUtils::getSetting("listener/cookiePath").toByteArray();
        QByteArray cookieComment=TextLabUtils::getSetting("listener/cookieComment").toByteArray();
        QByteArray cookieDomain=TextLabUtils::getSetting("listener/cookieDomain").toByteArray();
        HttpSession session(true);
        qDebug("HttpSessionStore: create new session with ID %s",session.getId().data());
        sessions.insert(session.getId(),session);
        response.setCookie(HttpCookie(cookieName,session.getId(),expirationTime/1000,cookiePath,cookieComment,cookieDomain));
        return session;
    }
    // Return a null session

    return HttpSession();
}

HttpSession HttpSessionStore::getSession(const QByteArray id)
{
    QMutexLocker locker(&mutex);
    HttpSession session=sessions.value(id);
    session.setLastAccess();
    return session;
}

void HttpSessionStore::sessionTimerEvent()
{
    QMutexLocker locker(&mutex);
    qint64 now=QDateTime::currentMSecsSinceEpoch();
    QMap<QByteArray,HttpSession>::iterator i = sessions.begin();
    while (i != sessions.end())
    {
        QMap<QByteArray,HttpSession>::iterator prev = i;
        ++i;
        HttpSession session=prev.value();
        qint64 lastAccess=session.getLastAccess();
        if (now-lastAccess>expirationTime)
        {
            qDebug("HttpSessionStore: session %s expired",session.getId().data());
            sessions.erase(prev);
        }
    }
}


/** Delete a session */
void HttpSessionStore::removeSession(HttpSession session)
{
    QMutexLocker locker(&mutex);
    sessions.remove(session.getId());
}
